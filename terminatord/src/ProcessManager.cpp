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

bool ProcessManager::IgnoredId(long user)
{
    int curr = 0;
    while (curr < Configuration::IgnoreCount)
    {
        if (user == Configuration::Ignore[curr])
        {
            return true;
        }
        curr++;
    }
    return false;
}

//! Convert task to name
//! @param task process info
string ProcessManager::Name(proc_t * task)
{
    return Core::Long2String(task->tid) + " " + task->cmd + " with usage of memory: "
              + Core::Long2String(task->resident * 4) + "KB with priority " + Core::Long2String(task->nice)
              + " owned by user (id: " + Core::Long2String(task->euid) + ") ";
}

//! Kill processes that are exceeding the limits
void ProcessManager::KillExcess()
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t proc_info;
    // zero out the allocated proc_info memory
    memset(&proc_info, 0, sizeof(proc_info));

    Core::DebugLog("Looking for processes that exceed the hard or soft limit", 10);

    while (readproc(proc, &proc_info) != NULL)
    {
        if (proc_info.tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " which is current instance of this daemon");
            continue;
        }

        if (!Configuration::KillRoot && (int)proc_info.euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by root", 6);
            }
            continue;
        }

        if (IgnoredId(proc_info.euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by ignored account: " + Core::int2String(proc_info.euid), 3);
            continue;
        }

        // check if this process is using most memory
        if ( proc_info.resident * 4 > ((long)Configuration::HardMemoryLimitMB * 1024) )
        {
            Core::Log("Exceeded hard limit - process " + Name(&proc_info) + " killing now");

            KillProc((pid_t)proc_info.tid, true);
        } else if ( proc_info.resident * 4 > ((long)Configuration::SoftMemoryLimitMB * 1024 ))
        {
            Core::Log("Exceeded soft limit - process " + Name(&proc_info) + " killing now");

            KillProc((pid_t)proc_info.tid, false);
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(&proc_info));
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

    while (readproc(proc, &proc_info) != NULL)
    {
        if (!Configuration::KillRoot && proc_info.euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by root", 6);
            }
            continue;
        }

        if (proc_info.tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " which is current instance of this daemon");
            continue;
        }

        if (IgnoredId(proc_info.euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by ignored account: " + Core::int2String(proc_info.euid), 2);
            continue;
        }

        // check if this process is using most memory
        if ( proc_info.resident * 4 > ((long)Configuration::HardMemoryLimitMB * 1024 ))
        {
            Core::Log("WARNING: Exceeded hard limit - process " + Name(&proc_info));
        } else if ( proc_info.resident * 4 > ((long)Configuration::SoftMemoryLimitMB * 1024 ))
        {
            Core::Log("WARNING: Exceeded soft limit - process " + Name(&proc_info));
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(&proc_info));
            }
        }
    }

    closeproc(proc);
    return;
}

//! Kill process which is eating most
void ProcessManager::KillHighest(bool hard)
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t proc_info;
    // zero out the allocated proc_info memory
    memset(&proc_info, 0, sizeof(proc_info));

    proc_t highest;
    long current_highest = 0;
    int current_score = -100;

    Core::DebugLog("Looking for a best candidate");

    while (readproc(proc, &proc_info) != NULL)
    {
        if (!Configuration::KillRoot && (int)proc_info.euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by root", 6);
            }
            continue;
        }

        if (IgnoredId(proc_info.euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " owned by ignored account: " + Core::int2String(proc_info.euid), 2);
            continue;
        }

        if (proc_info.tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info.tid) + " which is current instance of this daemon");
            continue;
        }

        int score = 0;

        // if it's a root process, decrease the score by 10

        if (proc_info.euser == 0)
        {
            score -= 10;
        }

        score = score + (int)proc_info.nice;

        // check if this process is using most memory
        if ( proc_info.resident * 4 > current_highest )
        {
            current_highest = proc_info.resident * 4;
            score += 10;
        }

        // if this process has highest score, we flag it for kill
        if ( score >= current_score )
        {
            highest = proc_info;
            current_score = score;
        } else
        {
            if (Configuration::Verbosity >= 12)
            {
                Core::DebugLog("Process " + Name(&proc_info) + "is eating less than highest candidate", 12);
            }
        }
    }

    if (current_score == -100)
    {
        Core::ErrorLog("Unable to find any process to kill. System is running OOM and I can't do anything to fix it.");
        return;
    }

    Core::Log("Most preferred process has score " + Core::int2String(current_score) + " : " + Name(&highest) +  " killing now");

    KillProc((pid_t)highest.tid, hard);

    closeproc(proc);
    return;
}
