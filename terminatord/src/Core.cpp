 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/Item.h"
#include "../include/Writer.h"
#include "../include/Parser.h"
#include "../include/Configuration.h"
#include "../include/Core.h"

//! Convert a number to 00 format
//! @param n Number
string Core::DecimalNumber(int n)
{
    ostringstream s;
    if (n >= 0 && n < 10)
    {
        s << "0" << n;
        return s.str();
    }
    s << n;
    return s.str();
}

//! Retrieve a current date and time
string Core::GetCurrentTime()
{
    // Print a current time
    time_t t = time(0);
    struct tm * now = localtime( & t );
    ostringstream s;
    s << now->tm_mday << ". " << DecimalNumber(now->tm_mon + 1) << ". " << (now->tm_year + 1900) << " "
    << DecimalNumber(now->tm_hour) << ":" << DecimalNumber(now->tm_min)
    << ":" << DecimalNumber(now->tm_sec);
    return s.str();
}

int Core::GetOom(pid_t pid)
{
    ifstream f;
    string path = "/proc/" + int2String((int)pid) + "/oom_score_adj";
    f.open(path);
    if(f.is_open())
    {
        string text;
        f >> text;
        f.close();
        return atoi(text.c_str());
    }
    f.close();
    return 0;
}

//! Convert long to string
//! @param number Item to convert
string Core::Long2String(unsigned long number)
{
    ostringstream s;
    s << number;
    return s.str();
}

//! Convert number to string
//! @param number Item to convert
string Core::int2String(int number)
{
    ostringstream s;
    s << number;
    return s.str();
}

//! Log
//! @param text Text to log
void Core::Log(string text)
{
    switch (Configuration::Logger)
    {
        // syslog
        case 0:
            openlog("terminatord", LOG_CONS, LOG_CRIT);
            syslog(LOG_INFO, "%s", text.c_str());
            closelog();
            return;
        // stdout
        case 1:
            cout << "[" << GetCurrentTime() << "] " << text << endl;
            return;
        case 2:
            Writer::Write(Configuration::LF, "[" + GetCurrentTime() + "] " + text);
            return;
    }
}

//! Log
//! @param text Text to log
void Core::ErrorLog(string text)
{
switch (Configuration::Logger)
    {
        // syslog
        case 0:
            openlog("terminatord", LOG_CONS, LOG_CRIT);
            syslog(LOG_ERR, "%s", text.c_str());
            closelog();
            return;
        // stdout
        case 1:
            cerr << "[" << GetCurrentTime() << "] ERROR: " << text << endl;
            return;
        case 2:
            Writer::Write(Configuration::LF, "[" + GetCurrentTime() + "] ERROR: " + text);
            return;
    }
}

//! Log
//! @param text Text to log
void Core::DebugLog(string text, unsigned int verbosity)
{
    if (verbosity <= Configuration::Verbosity)
    {
        Log("DEBUG (" + int2String(verbosity) + "): " + text);
    }
}
