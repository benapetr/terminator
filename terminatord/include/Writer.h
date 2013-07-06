 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.


#ifndef WRITER_H
#define WRITER_H

#include <iostream>
#include <list>
#include <fstream>
#include <list>
#include <mutex>
#include <pthread.h>
#include "Item.h"

using namespace std;

class Writer
{
    public:
        static void Write(string file, string text);
        static void Load();
        static void Terminate();
    protected:
    private:
        static void *Exec(void *threadid);
        static list<Item> DB;
        static pthread_t thread;
        static bool isRunning;
        static std::mutex &data_mut()
        {
            static std::mutex m;
            return m;
        }
};

#endif // WRITER_H
