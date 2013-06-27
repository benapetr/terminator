 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/ProcessManager.h"
#include "../include/Core.h"
#include "../include/Configuration.h"
#include "../include/Watcher.h"

//! Return if watcher is running, changing this to false terminate it
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
    return sys_info.totalram *(unsigned long long)sys_info.mem_unit;
}

//! Return free memory without buffers
unsigned long Watcher::GetFree()
{
    meminfo();
    if (Configuration::Swap)
    {
        return (kb_main_free + kb_main_cached + kb_main_buffers + kb_swap_free) * 1024;
    }
    return (kb_main_free + kb_main_cached + kb_main_buffers) * 1024;
}

//! Given that user enabled this it will check and kill processes that are exceeding system limit
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
            // in case we are still short of memory, and not in dry mode, we need to do something
            while (((unsigned long)Configuration::HardSystemLimitMB * 1024 * 1024) > free && !Configuration::DryMode)
            {
                Core::Log("System is out of memory, only " + Core::Long2String(free) + " bytes remaining, hard killing random process");
                ProcessManager::KillHighest(true);
            }
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

//! Check user limits
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

bool Watcher::ProtectSelf()
{
    if (Configuration::ProtectSelf)
    {
        string file ="/proc/" + Core::int2String(Configuration::pid) + "/oom_adj";
        Core::DebugLog("Removing own entry from kernel OOM");
        try
        {
            fstream filestr;
            filestr.open (file, fstream::in | fstream::out | fstream::trunc);
            if (filestr.rdstate() & std::ifstream::failbit)
            {
                Core::DebugLog("Error openning " + file, 0);
                return false;
            }
            filestr << "-17" <<endl;
            filestr.close();
        } catch (exception code)
        {
            Core::DebugLog("Unable to write to " + file + " error: " + code.what(), 0);
        }
        return true;
    }
    return false;
}

void Watcher::Start()
{
    Running = true;
    ProtectSelf();
    while (Running)
    {
        Core::DebugLog("Looping", 8);
        CheckSystemLimit();
        CheckUserLimit();
        // sleep for a specified value
        usleep(Configuration::Interval);
    }
}
