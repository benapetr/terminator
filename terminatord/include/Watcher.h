#ifndef WATCHER_H
#define WATCHER_H

#include <unistd.h>
#include <limits>
#include <sys/sysinfo.h>
#include <fstream>
#include <proc/readproc.h>
#include <proc/sysinfo.h>
#include <sys/resource.h>
#include <signal.h>

using namespace std;

class Watcher
{
    public:
        static void Start();
        static bool Running;
        static unsigned long GetMemTotal();
        static unsigned long GetFree();
    protected:
    private:
        static void CheckSystemLimit();
        static void CheckUserLimit();
};

#endif // WATCHER_H
