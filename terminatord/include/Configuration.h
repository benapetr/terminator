#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>

using namespace std;

class Configuration
{
    public:
        static string Version;
        static unsigned int Verbosity;
        static bool ReportOOM;
        static bool KillOOM;
        static unsigned int MemoryLimitMB;
        static bool Daemon;
        static bool KillRoot;
        static int Interval;
    protected:
    private:
};

#endif // CONFIGURATION_H
