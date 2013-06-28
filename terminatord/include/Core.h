 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef CORE_H
#define CORE_H

#include <sstream>
#include <syslog.h>
#include <iostream>

using namespace std;

class Core
{
    public:
        static void Log(string text);
        static void DebugLog(string text, unsigned int verbosity = 1);
        static void ErrorLog(string text);
        static string int2String(int number);
        static string Long2String(unsigned long number);
        static int GetOom(pid_t pid);
    protected:
    private:
        static string GetCurrentTime();
        static string DecimalNumber(int n);
};

#endif // CORE_H
