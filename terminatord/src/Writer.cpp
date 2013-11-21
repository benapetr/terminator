 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

// this file writes logs to file

#include "../include/Writer.h"

//! Database of objects that writer needs to write
std::list<Item> Writer::DB;
//! Whether writer is running
bool Writer::isRunning = true;
pthread_t Writer::thread;

std::mutex &Writer::data_mut()
{
    static std::mutex m;
    return m;
}

//! This function is exec of thread of writer
void *Writer::Exec(void *threadid)
{
    while(Writer::isRunning)
    {
        // lock the array and copy it
        Writer::data_mut().lock();
        list<Item> tm (Writer::DB);
        Writer::DB.clear();
        Writer::data_mut().unlock();

        // foreach write every single record
        list<Item>::iterator iter;
        for(iter = tm.begin(); iter != tm.end(); iter++)
        {
            if (Configuration::Verbosity > 1)
            {
                Core::DebugLog( "Writing to " + iter->File );
            }
            try
            {
                fstream filestr;
                filestr.open (iter->File, fstream::in | fstream::out | fstream::app);
                if (filestr.rdstate() & std::ifstream::failbit)
                {
                    Core::DebugLog("Error openning " + iter->File, 0);
                    continue;
                }
                filestr << iter->Text <<endl;
                filestr.close();
            } catch (exception code)
            {
                Core::DebugLog("Unable to write to " + iter->File + " error: " + code.what(), 0);
            }
        }

        sleep(2);
    }
    pthread_exit(NULL);
}

//! Called to stop the writer
void Writer::Terminate()
{
    Writer::isRunning = false;
    // check if db is empty
    bool wait = true;
    while (wait)
    {
        Writer::data_mut().lock();
        if (Writer::DB.size() > 0)
        {
            Core::DebugLog("There are still data in memory, waiting for writer to finish");
            sleep(2);
            continue;
        }
        Writer::data_mut().unlock();
        wait = false;
    }
    // grace time for buffer to empty
    sleep(1);
    Core::DebugLog("Terminating writer");
    return;
}

void Writer::Load()
{
    int r = pthread_create(&Writer::thread, NULL, Writer::Exec, (void *)1);
    Core::DebugLog("Initialized the writer thread");
    if (r)
    {
        cout << "Error:unable to create thread," << r << endl;
    }
}

//! Write an item to disk
void Writer::Write(string file, string text)
{
    if (text == "")
    {
        Core::DebugLog("Not writing empty text to " + file);
        return;
    }
    // lock array
    std::lock_guard<std::mutex> _(Writer::data_mut());
    // write this line
    DB.push_back(Item(file,text));
}

