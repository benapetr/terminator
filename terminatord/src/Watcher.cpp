#include <unistd.h>
#include <limits>
#include <sys/sysinfo.h>
#include <fstream>
#include <proc/readproc.h>
#include <sys/resource.h>
#include <signal.h>
#include "../include/ProcessManager.h"
#include "../include/Core.h"
#include "../include/Configuration.h"
#include "../include/Watcher.h"

bool Watcher::Running = false;

unsigned long Watcher::GetMemTotal()
{
    struct sysinfo sys_info;
    if(sysinfo(&sys_info) != 0)
    {
        Core::ErrorLog("Unable to retrieve system information");
        Running = false;
        return 0;
    }
    return sys_info.totalram;
}

unsigned long Watcher::GetFree()
{
    struct sysinfo sys_info;
    if(sysinfo(&sys_info) != 0)
    {
        Core::ErrorLog("Unable to retrieve system information");
        Running = false;
        return 0;
    }
    return sys_info.freeram;
}

void Watcher::CheckSystemLimit()
{
    Core::DebugLog("Checking system limit", 2);
    if (Configuration::HardSystemLimitMB == 0 && Configuration::SoftSystemLimitMB == 0)
    {
        Core::DebugLog("Skipping system limit check, because it's set to 0", 2);
        return;
    }
    // check hard limit
    if (Configuration::HardSystemLimitMB > 0)
    {
        unsigned long free = Watcher::GetFree();
        if (((unsigned long)Configuration::HardSystemLimitMB * 1024 * 1024) > free)
        {
            Core::Log("System is out of memory, only " + Core::Long2String(free) + " bytes remaining, hard killing random process");
            ProcessManager::KillHighest(true);
        } else
        {
            if (Configuration::Verbosity >= 8)
            {
                Core::Log("System is not exceeding hard limit, using " + Core::Long2String(free) + " bytes of ram");
            }
        }
    }

    if (Configuration::SoftSystemLimitMB > 0)
    {
        unsigned long free = Watcher::GetFree();
        if (((unsigned long)Configuration::SoftSystemLimitMB * 1024 * 1024) > free)
        {
            Core::Log("System is out of memory, only " + Core::Long2String(free) + " bytes remaining, killing random process");
            ProcessManager::KillHighest(false);
        } else
        {
            if (Configuration::Verbosity >= 8)
            {
                Core::Log("System is not exceeding limit, using " + Core::Long2String(free) + " bytes of ram");
            }
        }
    }
}

void Watcher::CheckUserLimit()
{
    Core::DebugLog("Checking processes", 2);
    if (Configuration::SoftMemoryLimitMB == 0 && Configuration::HardMemoryLimitMB == 0)
    {
        Core::DebugLog("Skipping check for user processes", 2);
        return;
    }
    if (Configuration::KillOOM)
    {
        ProcessManager::KillExcess();
        return;
    }
    if (Configuration::ReportOOM)
    {
        ProcessManager::WarnExcess();
    }
}

void Watcher::Start()
{
    Running = true;
    while (Running)
    {
        Core::DebugLog("Looping", 8);
        CheckSystemLimit();
        CheckUserLimit();
        // sleep for a specified value
        usleep(Configuration::Interval);
    }
}
