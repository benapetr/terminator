 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

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

string Core::Long2String(unsigned long number)
{
    ostringstream s;
    s << number;
    return s.str();
}

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
    cout << "[" << GetCurrentTime() << "] " << text << endl;
}

//! Log
//! @param text Text to log
void Core::ErrorLog(string text)
{
    cerr << "[" << GetCurrentTime() << "] ERROR: " << text << endl;
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
