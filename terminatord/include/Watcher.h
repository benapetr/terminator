 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef WATCHER_H
#define WATCHER_H

#include <limits>
#include <sys/sysinfo.h>
#include <fstream>
#include <proc/readproc.h>
#include <proc/sysinfo.h>
#include <signal.h>
#include "ProcessManager.h"
#include "Core.h"
#include "Configuration.h"

using namespace std;

namespace terminator
{
    class Watcher
    {
        public:
            static void Start();
            static bool Running;
            static unsigned long GetMemTotal();
            static unsigned long GetFree();
            static bool ProtectSelf();
        protected:
        private:
            static void CheckSystemLimit();
            static void CheckUserLimit();
    };
}

#endif // WATCHER_H
