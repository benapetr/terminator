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
        static unsigned int HardMemoryLimitMB;
        static unsigned int SoftMemoryLimitMB;
        static bool Daemon;
        static bool KillRoot;
        static int Interval;
        static unsigned int HardSystemLimitMB;
        static unsigned int SoftSystemLimitMB;
        static unsigned long MemoryTotal;
        static bool DryMode;
    protected:
    private:
};

#endif // CONFIGURATION_H