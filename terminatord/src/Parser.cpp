 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include <cstdlib>
#include "../include/Configuration.h"
#include "../include/Parser.h"
#include "../include/Core.h"

Parser::Parser(int c, char *v[])
{
    argc = c;
    argv = v;
}

//! Print a help for command
void Parser::ShowHelp()
{
    cout << "Usage: terminatord [hkvd]" << endl << endl;
    cout << "Terminator daemon is killing processes that exceed some limit" << endl << endl;
    cout << "Arguments:" << endl;
    cout << " -h [--help]: Display this help" << endl;
    cout << " -k [--kill]: Will kill processes if they exceed the limits" << endl;
    cout << " --soft limit in MB: Set a soft memory limit" << endl;
    cout << " --hard limit in MB: Set a hard memory limit" << endl;
    cout << " --ssoft limit in MB: Set a soft system memory limit" << endl;
    cout << " --shard limit in MB: Set a hard system memory limit" << endl;
    cout << " --root: Kill even system processes if they exceed the limit" << endl;
    cout << " --ignore 1,2...: Set a list of uid to ignore, separated by comma (with no spaces)" << endl;
    cout << " --dry: Never kill any process" << endl;
    cout << " --interval number: Set an interval between checks in microseconds" << endl;
    cout << " --quiet: Don't report processes that exceed the limits" << endl;
    cout << " -d: Run in a daemon mode" << endl;
    cout << " -v [--verbose]: Increase verbosity" << endl << endl;
    cout << "Terminatord version " << Configuration::Version << endl << endl;
    cout << "This software is open source licensed under GPL v. 3, see https://github.com/benapetr/terminator for source code" << endl;
}

bool Parser::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//! Parse the parameters for program, return true if program is supposed to stop
bool Parser::Parse()
{
    if (argc < 2)
    {
        cout << "Usage: terminatord [hkvd]" << endl;
        return true;
    }

    int curr = 1;
    while (curr < argc)
    {
        string parameter = argv[curr];
        curr++;
        if (parameter == "-h" || parameter == "--help")
        {
            ShowHelp();
            return true;
        }
        if (parameter == "-k" || parameter == "--kill")
        {
            Configuration::KillOOM = true;
            continue;
        }
        if (parameter == "-v" || parameter == "--verbose")
        {
            Configuration::Verbosity++;
            continue;
        }
        if (parameter == "--ssoft")
        {
            string n = argv[curr];
            if (!is_number(n))
            {
                Core::ErrorLog("Provided argument is not a number: " + n);
                return true;
            }
            Configuration::SoftSystemLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--hard")
        {
            string n = argv[curr];
            if (!is_number(n))
            {
                Core::ErrorLog("Provided argument is not a number: " + n);
                return true;
            }
            Configuration::HardMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--soft")
        {
            string n = argv[curr];
            if (!is_number(n))
            {
                Core::ErrorLog("Provided argument is not a number: " + n);
                return true;
            }
            Configuration::SoftMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--shard")
        {
            string n = argv[curr];
            if (!is_number(n))
            {
                Core::ErrorLog("Provided argument is not a number: " + n);
                return true;
            }
            Configuration::HardSystemLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--interval")
        {
            string n = argv[curr];
            if (!is_number(n))
            {
                Core::ErrorLog("Provided argument is not a number: " + n);
                return true;
            }
            Configuration::Interval = atoi(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--root")
        {
            Configuration::KillRoot = true;
            continue;
        }
        if (parameter == "--dry")
        {
            Configuration::DryMode = true;
            continue;
        }
        if (parameter == "-a" || parameter == "--hard")
        {
            Configuration::HardMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "-d")
        {
            Configuration::Daemon = true;
            continue;
        }
        if (parameter == "--version")
        {
            cout << "Terminator version: " << Configuration::Version << endl;
            return true;
        }
        if (parameter == "--quiet")
        {
            Configuration::ReportOOM = false;
            continue;
        }
        if (parameter == "--ignore")
        {
            string list = argv[curr];
            while (list.find(",") != string::npos)
            {
                string id = list.substr(0, list.find(","));
                list = list.substr(list.find(",") + 1);
                if (is_number(id))
                {
                    Configuration::Ignore[Configuration::IgnoreCount] = atoi(id.c_str());
                    Configuration::IgnoreCount++;
                }
            }
            if (is_number(list))
            {
                Configuration::Ignore[Configuration::IgnoreCount] = atoi(list.c_str());
                Configuration::IgnoreCount++;
            }
            curr++;
            continue;
        }
        if (parameter.size() > 1 && parameter.substr(0, 1) == "-")
        {
            unsigned int c = 1;
            while (c < parameter.size())
            {
                if ( parameter[c] == 'v' )
                {
                    Configuration::Verbosity++;
                }
                if ( parameter[c] == 'h' )
                {
                    ShowHelp();
                    return true;
                }
                if ( parameter[c] == 'k' )
                {
                    Configuration::KillOOM = true;
                }
                if ( parameter[c] == 'd' )
                {
                    Configuration::Daemon = true;
                }
                c++;
            }
            continue;
        }
    }
    return false;
}


