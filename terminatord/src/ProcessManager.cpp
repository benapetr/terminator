 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/Core.h"
#include "../include/ProcessManager.h"
#include "../include/Watcher.h"
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

void ProcessManager::Exec(proc_t* proc)
{
    if (Configuration::Exec)
    {
        Core::DebugLog("Executing " + Configuration::ExecPath, 2);
        string command = Configuration::ExecPath + " " +
                            Core::int2String(proc->tid) +
                            " " + proc->cmd +
                            " " + Core::int2String(proc->euid) +
                            " " + Core::Long2String(proc->resident * 4) +
                            " " + Core::Long2String(Watcher::GetFree());
        system(command.c_str());
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

    Core::DebugLog("Looking for processes that exceed the hard or soft limit", 10);

    proc_t *proc_info;

    while (true)
    {
        proc_info = readproc(proc, NULL);
        if (proc_info == NULL)
        {
            break;
        }
        if (proc_info->tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " which is current instance of this daemon", 8);
            freeproc(proc_info);
            continue;
        }

        if (!Configuration::KillRoot && (int)proc_info->euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by root", 6);
            }
            freeproc(proc_info);
            continue;
        }

        if (IgnoredId(proc_info->euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by ignored account: " + Core::int2String(proc_info->euid), 3);
            freeproc(proc_info);
            continue;
        }

        // check if this process is using most memory
        if ( proc_info->resident * 4 > ((long)Configuration::HardMemoryLimitMB * 1024) )
        {
            Core::Log("Exceeded hard limit - process " + Name(proc_info) + " killing now");

            KillProc((pid_t)proc_info->tid, true);
            Exec(proc_info);
        } else if ( proc_info->resident * 4 > ((long)Configuration::SoftMemoryLimitMB * 1024 ))
        {
            Core::Log("Exceeded soft limit - process " + Name(proc_info) + " killing now");

            KillProc((pid_t)proc_info->tid, false);
            Exec(proc_info);
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(proc_info));
            }
        }
        freeproc(proc_info);
    }

    closeproc(proc);
    return;
}

void ProcessManager::WarnExcess()
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t * proc_info;

    Core::DebugLog("Looking for processes that exceed the hard or soft limit", 10);

    while (true)
    {
        proc_info = readproc(proc, NULL);
        if (proc_info == NULL)
        {
            break;
        }
        if (!Configuration::KillRoot && proc_info->euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by root", 6);
            }
            freeproc(proc_info);
            continue;
        }

        if (proc_info->tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " which is current instance of this daemon", 6);
            freeproc(proc_info);
            continue;
        }

        if (IgnoredId(proc_info->euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by ignored account: " + Core::int2String(proc_info->euid), 2);
            freeproc(proc_info);
            continue;
        }

        // check if this process is using most memory
        if ( proc_info->resident * 4 > ((long)Configuration::HardMemoryLimitMB * 1024 ))
        {
            Core::Log("WARNING: Exceeded hard limit - process " + Name(proc_info));
        } else if ( proc_info->resident * 4 > ((long)Configuration::SoftMemoryLimitMB * 1024 ))
        {
            Core::Log("WARNING: Exceeded soft limit - process " + Name(proc_info));
        } else
        {
            if (Configuration::Verbosity > 12)
            {
                Core::DebugLog("Not exceeded any limit " + Name(proc_info));
            }
        }
        freeproc(proc_info);
    }

    closeproc(proc);
    return;
}

//! Kill process which is eating most
void ProcessManager::KillHighest(bool hard)
{
    PROCTAB* proc = openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLMEM);
    proc_t* proc_info;
    // zero out the allocated proc_info memory
    //memset(&proc_info, 0, sizeof(proc_info));

    proc_t highest;
    long current_highest = 0;
    int current_score = -100;

    Core::DebugLog("Looking for a best candidate");

    while (true)
    {
        proc_info = readproc(proc, NULL);
        if (proc_info == NULL)
        {
            break;
        }
        if (!Configuration::KillRoot && (int)proc_info->euid == 0)
        {
            if (Configuration::Verbosity >= 6)
            {
                Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by root", 6);
            }
            freeproc(proc_info);
            continue;
        }

        if (IgnoredId(proc_info->euid))
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " owned by ignored account: " + Core::int2String(proc_info->euid), 2);
            freeproc(proc_info);
            continue;
        }

        if (proc_info->tid == Configuration::pid && !Configuration::KillSelf)
        {
            Core::DebugLog("Ignoring " + Core::int2String(proc_info->tid) + " which is current instance of this daemon", 6);
            freeproc(proc_info);
            continue;
        }

        int score = 0;

        // if it's a root process, decrease the score by 10

        if (proc_info->euser == 0)
        {
            score -= 10;
        }

        score = score + (int)proc_info->nice;

        int badness_score = Core::GetOom(proc_info->tid);

        if (badness_score <= -17)
        {
            // ignore process
            freeproc(proc_info);
            continue;
        }

        if (badness_score != 0)
        {
            score = score + badness_score;
        }

        // check if this process is using most memory
        if ( proc_info->resident * 4 > current_highest )
        {
            current_highest = proc_info->resident * 4;
            score += 10;
        }

        // if this process has highest score, we flag it for kill
        if ( score >= current_score )
        {
            highest = *proc_info;
            current_score = score;
        } else
        {
            if (Configuration::Verbosity >= 12)
            {
                Core::DebugLog("Process " + Name(proc_info) + "is eating less than highest candidate", 12);
            }
        }
        freeproc(proc_info);
    }

    if (current_score == -100)
    {
        Core::ErrorLog("Unable to find any process to kill. System is running OOM and I can't do anything to fix it.");
        closeproc(proc);
        return;
    }

    Core::Log("Most preferred process has score " + Core::int2String(current_score) + " : " + Name(&highest) +  " killing now");

    Exec(&highest);
    KillProc((pid_t)highest.tid, hard);

    closeproc(proc);
    return;
}
