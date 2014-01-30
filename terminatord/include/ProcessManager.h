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

#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <proc/readproc.h>
#include <sys/stat.h>
#include "../include/Core.h"
#include "../include/Watcher.h"
#include "../include/Configuration.h"

using namespace std;

namespace terminator
{
    class ProcessManager
    {
        public:
            static void KillHighest(bool hard);
            static string Name(proc_t *task);
            static void WarnExcess();
            static void KillExcess();
            static void KillProc(pid_t pd, bool hard);
            static unsigned int KillExec(proc_t* proc);
        protected:
            static void Exec(proc_t* proc);
        private:
            static bool IgnoredId(long user);
    };
}

#endif // PROCESSMANAGER_H
