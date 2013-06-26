#include "../include/Core.h"
#include "../include/ProcessManager.h"
#include "../include/Configuration.h"

void ProcessManager::KillProc(pid_t pd, bool hard)
{
    if (Configuration::DryMode)
    {
        Core::DebugLog("Not killing in dry mode");
        return;
    }
    int result;
    if ((int)pd <= 0)
    {
        Core::DebugLog("Invalid pid " + Core::int2String((int)pd));
        return;
    }
    if (!hard)
    {
        result = kill(pd, SIGTERM);
    } else
    {
        result = kill(pd, SIGKILL);
    }
    if (result != 0)
    {
        Core::ErrorLog("Can't kill " + Core::int2String((int)pd));
    }
}

string ProcessManager::Name(proc_t * task)
{
    return Core::Long2String(task->tid) + " " + task->cmd + " with usage of memory: "
              + Core::Long2String(task->resident * 4) + " with priority " + Core::Long2String(task->nice)
              + " owned by user (id: " + Core::Long2String(task->euid) + ") ";
}

void ProcessManager::KillExcess()
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t proc_info;
    // zero out the allocated proc_info memory
    memset(&proc_info, 0, sizeof(proc_info));

    Core::DebugLog("Looking for processes that exceed the hard or soft limit", 10);

    proc_t* highest;

    while (readproc(proc, &proc_info) != NULL)
    {
        highest = &proc_info;
        if (!Configuration::KillRoot && (int)proc_info.euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(highest->tid) + " owned by root", 6);
            }
            continue;
        }

        // check if this process is using most memory
        if ( proc_info.resident * 4 > Configuration::HardMemoryLimitMB * 1024 )
        {
            Core::Log("Exceeded hard limit - process " + Name(highest) + " killing now");

            KillProc((pid_t)proc_info.tid, true);
        } else if ( proc_info.resident * 4 > Configuration::SoftMemoryLimitMB * 1024 )
        {
            Core::Log("Exceeded soft limit - process " + Name(highest) + " killing now");

            KillProc((pid_t)proc_info.tid, false);
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(highest));
            }
        }
    }

    closeproc(proc);
    return;
}

void ProcessManager::WarnExcess()
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t proc_info;
    // zero out the allocated proc_info memory
    memset(&proc_info, 0, sizeof(proc_info));

    Core::DebugLog("Looking for processes that exceed the hard or soft limit");

    proc_t* highest;

    while (readproc(proc, &proc_info) != NULL)
    {
        highest = &proc_info;
        if (!Configuration::KillRoot && proc_info.euser == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(highest->tid) + " owned by root", 6);
            }
            continue;
        }

        // check if this process is using most memory
        if ( proc_info.resident * 4 > Configuration::HardMemoryLimitMB * 1024 )
        {
            Core::Log("WARNING: Exceeded hard limit - process " + Name(highest));
        } else if ( proc_info.resident * 4 > Configuration::SoftMemoryLimitMB * 1024 )
        {
            Core::Log("WARNING: Exceeded soft limit - process " + Name(highest));
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(highest));
            }
        }
    }

    closeproc(proc);
    return;
}

void ProcessManager::KillHighest(bool hard)
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT);
    proc_t proc_info;
    // zero out the allocated proc_info memory
    memset(&proc_info, 0, sizeof(proc_info));

    proc_t* highest;
    unsigned int current_highest = 0;

    Core::DebugLog("Looking for a best candidate");

    while (readproc(proc, &proc_info) != NULL)
    {
        if (!Configuration::KillRoot && proc_info.euser == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by root", 6);
            }
            continue;
        }

        // check if this process is using most memory
        if ( proc_info.resident * 4 > current_highest )
        {
            current_highest = proc_info.resident * 4;
            highest = &proc_info;
        }
    }

    if (current_highest == 0)
    {
        Core::ErrorLog("Unable to find any process to kill. System is running OOM and I can't do anything to fix it.");
        return;
    }

    Core::Log("Most prefered is process " + Name(highest) +  " killing now");

    KillProc((pid_t)highest->tid, hard);

    closeproc(proc);
    return;
}
