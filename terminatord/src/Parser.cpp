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
    cout << " --dry: Never kill any process" << endl;
    cout << " -d: Run in a daemon mode" << endl;
    cout << " -v [--verbose]: Increase verbosity" << endl << endl;
    cout << "Terminatord version " << Configuration::Version << endl << endl;
    cout << "This software is open source licensed under GPL v. 3, see https://github.com/benapetr/terminator for source code" << endl;
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
            Configuration::SoftSystemLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--hard")
        {
            Configuration::HardMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--soft")
        {
            Configuration::SoftMemoryLimitMB = atol(argv[curr]);
            curr++;
            continue;
        }
        if (parameter == "--shard")
        {
            Configuration::HardSystemLimitMB = atol(argv[curr]);
            curr++;
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


