#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <proc/readproc.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class ProcessManager
{
    public:
        static void KillHighest(bool hard);
        static string Name(proc_t *task);
        static void WarnExcess();
        static void KillExcess();
        static void KillProc(pid_t pd, bool hard);
    protected:
    private:
        static bool IgnoredId(long user);
};

#endif // PROCESSMANAGER_H
