 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <proc/readproc.h>
#include <sys/stat.h>
#include "../include/Core.h"
#include "../include/Watcher.h"
#include "../include/Configuration.h"

using namespace std;

namespace terminator
{
    //! When a process is killed it may need some time for it to take effect, in order to prevent
    //! killing it repeatedly in a loop, we store it into a cache using this
    class BufferItem
    {
        public:
            BufferItem(pid_t pid);
            BufferItem(BufferItem *b);
            pid_t Pid;
            time_t Time;
    };

    namespace ProcessManager
    {
        void KillHighest(bool hard);
        string Name(proc_t *task);
        void WarnExcess();
        void KillExcess();
        void KillProc(pid_t pd, bool hard);
        unsigned int KillExec(proc_t* proc);
        void Exec(proc_t* proc);
        bool IgnoredId(long user);
        void ClearKilled();
        BufferItem *IsKilled(pid_t pid);
    }
}

#endif // PROCESSMANAGER_H
