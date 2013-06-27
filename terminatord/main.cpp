 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <proc/readproc.h>
#include "include/Configuration.h"
#include "include/Parser.h"
#include "include/Core.h"
#include "include/Watcher.h"

using namespace std;

int main(int argc, char *argv[])
{
    Parser* parser = new Parser(argc, argv);
    if (parser->Parse())
    {
        return 0;
    }
    delete parser;
    if (!Configuration::Daemon)
    {
        Core::ErrorLog("Nothing to do");
        return 2;
    }
    Core::Log("Terminatord v" + Configuration::Version);
    Core::DebugLog("Loading configuration");
    Configuration::MemoryTotal = Watcher::GetMemTotal();
    if (Configuration::Verbosity >= 1)
    {
        cout << "Debugging: " << Configuration::Verbosity << endl;
        cout << "Detected ram: " << Configuration::MemoryTotal << endl;
        cout << "Current ram:" << Watcher::GetFree() << endl;
    }
    Watcher::Start();
    return 0;
}
