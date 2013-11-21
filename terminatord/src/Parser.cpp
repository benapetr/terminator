 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#include "../include/Parser.h"

//! Constructor
//! @param c count of params
//! @param v list of items
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
    cout << " -k [--kill]: Will kill processes if they exceed the limits, if this is not enabled," << endl;
    cout << "              the processes that exceed the limit will be only reported" << endl;
    cout << " --soft limit in MB: Set a soft memory limit for processes (will be killed if they exceed it)" << endl;
    cout << " --hard limit in MB: Set a hard memory limit for processes (will be killed with -9 if they exceed it)" << endl;
    cout << " --ssoft limit in MB: Set a soft system memory limit (daemon will attempt to clear memory" << endl;
    cout << "                      when system has less than this amount of ram" << endl;
    cout << " --shard limit in MB: Set a hard system memory limit" << endl;
    cout << " --root: Kill even system processes if they exceed the limit" << endl;
    cout << " --ignore 1,2...: Set a list of uid to ignore, separated by comma (with no spaces)" << endl;
    cout << " --dry: Never kill any process" << endl;
    cout << " --interval number: Set an interval between checks in microseconds" << endl;
    cout << " --quiet: Don't report processes that exceed the limits" << endl;
    cout << " --pid file: Write a pid to given file" << endl;
    cout << " --exec file: Execute a given file on kill with parameters: <pid> <cmd> <userid> <memoryused in kilo-bytes> <memoryfree in bytes>" << endl;
    cout << " --syslog: write to syslog" << endl;
    cout << " --noprotectoom: doesn't protect self against the kernel oom killer" << endl;
    cout << " --testexec: Run a command before kill of process with parameters: <pid> <cmd> <userid> <memoryused in kilo-bytes> <memory free in bytes>" << endl;
    cout << "             if this command return non zero, the process will not be killed" << endl;
    cout << " --ignoreswap: will not count swap to free memory (will kill processes even if there is space in swap)" << endl;
    cout << " --log file: write to a file" << endl;
    cout << " -d: Run in a daemon mode" << endl;
    cout << " -v [--verbose]: Increase verbosity" << endl << endl;
    cout << "Terminatord version " << Configuration::Version << endl << endl;
    cout << "This software is open source licensed under GPL v. 3, see https://github.com/benapetr/terminator for source code" << endl;
}

bool Parser::NextIsNumber(int next)
{
    if (argc <= next)
    {
        return false;
    }
    string s = argv[next];
    return is_number(s);
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
            if (!NextIsNumber(curr))
            {
                Core::ErrorLog("Provided argument is not a number or is missing");
                return true;
            }
            string n = argv[curr];
            Configuration::SoftSystemLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--hard")
        {
            if (!NextIsNumber(curr))
            {
                Core::ErrorLog("Provided argument is not a number or is missing");
                return true;
            }
            string n = argv[curr];
            Configuration::HardMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--soft")
        {
            if (!NextIsNumber(curr))
            {
                Core::ErrorLog("Provided argument is not a number or is missing");
                return true;
            }
            string n = argv[curr];
            Configuration::SoftMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--shard")
        {
            if (!NextIsNumber(curr))
            {
                Core::ErrorLog("Provided argument is not a number or is missing");
                return true;
            }
            string n = argv[curr];
            Configuration::HardSystemLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--interval")
        {
            if (!NextIsNumber(curr))
            {
                Core::ErrorLog("Provided argument is not a number or is missing");
                return true;
            }
            string n = argv[curr];
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
        if (parameter == "--log")
        {
            if (argc <= curr)
            {
                Core::ErrorLog("You need to provide a path to file");
                return true;
            }
            Configuration::LF = argv[curr];
            curr++;
            Configuration::Logger = 2;
            continue;
        }
        if (parameter == "--syslog")
        {
            Configuration::Logger = 0;
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
        if (parameter == "--ignoreswap")
        {
            Configuration::Swap = false;
            continue;
        }
        if (parameter == "--exec")
        {
            if (argc <= curr)
            {
                Core::ErrorLog("You need to provide a path to binary");
                return true;
            }
            Configuration::ExecPath = argv[curr];
            curr++;
            Configuration::Exec = true;
            continue;
        }
        if (parameter == "--pid")
        {
            if (argc <= curr)
            {
                Core::ErrorLog("You need to provide a path to file");
                return true;
            }
            string fl = argv[curr];
            try
            {
                fstream filestr;
                filestr.open (argv[curr], fstream::in | fstream::out | fstream::trunc);
                if (filestr.rdstate() & std::ifstream::failbit)
                {
                    Core::ErrorLog("Error openning " + fl);
                    return true;
                }
                filestr << Configuration::pid <<endl;
                filestr.close();
            } catch (exception code)
            {
                Core::ErrorLog("Unable to write to " + fl + " error: " + code.what());
                return true;
            }
            curr++;
            continue;
        }
        if (parameter == "--noprotectoom")
        {
            Configuration::ProtectSelf = false;
            continue;
        }
        if (parameter == "--testexec")
        {
            if (argc <= curr)
            {
                Core::ErrorLog("You need to provide a path to binary");
                return true;
            }
            Configuration::KillEP = argv[curr];
            curr++;
            Configuration::KillExec = true;
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


