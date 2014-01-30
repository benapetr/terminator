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

namespace terminator
{
    enum LoggingSystem
    {
        Syslog,
        Stdout,
        File
    };

    class Configuration
    {
        public:
            //! Version
            static string Version;
            //! Verbosity
            static unsigned int Verbosity;
            //! Whether processes should be reported when they exceed certain memory limit
            static bool ReportOOM;
            //! Whether processes should be killed when they exceed certain memory limit
            static bool KillOOM;
            //! Value that specifies when processes needs to be killed
            static unsigned int HardMemoryLimitMB;
            //! Value that specifies when processes needs to be killed
            static unsigned int SoftMemoryLimitMB;
            //! Running as background process
            static bool Daemon;
            static string LF;
            //! Type of log subsystem to use by daemon
            static LoggingSystem Logger;
            static bool KillRoot;
            static int Interval;
            //! Kill itself when best candidate
            static bool KillSelf;
            //! Protect against kernel OOM killer
            static bool ProtectSelf;
            //! Execute this before kill of process
            static bool KillExec;
            //! Value that specifies when processes needs to be killed
            static unsigned int HardSystemLimitMB;
            //! Value that specifies when processes needs to be killed
            static unsigned int SoftSystemLimitMB;
            static unsigned long MemoryTotal;
            //! Path to program to launch before process is killed
            static string KillEP;
            static bool DryMode;
            //! Pid of current process
            static pid_t pid;
            //! List of uid to ignore
            static int Ignore[800];
            //! Number of items in ignore list
            static int IgnoreCount;
            static bool WritePid;
            static string PidFile;
            //! Start a program after process is killed
            static bool Exec;
            //! Whether swap should be considered as free memory too
            static bool Swap;
            //! Path to program to launch when process is killed
            static string ExecPath;
    };
}

#endif // CONFIGURATION_H
