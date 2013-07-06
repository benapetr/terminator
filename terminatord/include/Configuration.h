 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <unistd.h>
#include <iostream>

using namespace std;

class Configuration
{
    public:
        static string Version;
        static unsigned int Verbosity;
        static bool ReportOOM;
        static bool KillOOM;
        static unsigned int HardMemoryLimitMB;
        static unsigned int SoftMemoryLimitMB;
        static bool Daemon;
        static string LF;
        static int Logger;
        static bool KillRoot;
        static int Interval;
        static bool KillSelf;
        static bool ProtectSelf;
        static bool KillExec;
        static unsigned int HardSystemLimitMB;
        static unsigned int SoftSystemLimitMB;
        static unsigned long MemoryTotal;
        static string KillEP;
        static bool DryMode;
        static pid_t pid;
        static int Ignore[800];
        static int IgnoreCount;
        static bool Exec;
        static bool Swap;
        static string ExecPath;
    protected:
    private:
};

#endif // CONFIGURATION_H
