 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include <iostream>
#include <proc/readproc.h>
#include "include/Configuration.h"
#include "include/Parser.h"
#include "include/Writer.h"
#include "include/Core.h"
#include "include/Watcher.h"

using namespace std;
using namespace terminator;

int main(int argc, char *argv[])
{
    Parser* parser = new Parser(argc, argv);
    if (parser->Parse())
    {
        return 0;
    }
    delete parser;
    if (Configuration::Logger == terminator::File)
    {
        Writer::Load();
    }
    if (!Configuration::Daemon)
    {
        Core::ErrorLog("Nothing to do");
        return 2;
    }
    // Daemonize self
    /* Fork off the parent process */
    pid_t pid = fork();

    if (pid < 0)
    {
        Core::DebugLog("Parent: forked to pid: " + Core::int2String(pid));
        Core::ErrorLog("Failed to daemonize itself call to fork() failed with " + Core::int2String(errno));
        return 60;
    }

    if (pid > 0)
    {
        Core::DebugLog("Parent: forked to pid: " + Core::int2String(pid));
        return 0;
    }

    // patch that adjust the forked pid
    Configuration::pid = getpid();
    Core::DebugLog("Forked daemon: adjusted pid to " + Core::int2String(Configuration::pid));

    // write a pid to a file
    Core::WritePid();

    Core::Log("Terminatord v" + Configuration::Version);
    Core::DebugLog("Loading configuration");
    Configuration::MemoryTotal = Watcher::GetMemTotal();
    if (Configuration::Verbosity >= 1)
    {
        cout << "Debugging: " << Configuration::Verbosity << endl;
        cout << "Detected ram: " << Configuration::MemoryTotal << endl;
        cout << "Free ram: " << Watcher::GetFree() << endl;
        if (Configuration::KillOOM)
        {
            cout << "Will kill processes that exceed the limits" << endl;
        }
        if (Configuration::KillRoot)
        {
            cout << "Will kill root processes as well" << endl;
        }
        if (Configuration::Exec)
        {
            cout << "I will run on every kill: " << Configuration::ExecPath << endl;
        }
        if (Configuration::DryMode)
        {
            cout << "In fact, I will not kill anything, because of dry option" << endl;
        }
    }
    Watcher::Start();
    return 0;
}
