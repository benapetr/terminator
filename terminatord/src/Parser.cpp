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
    cout << "Usage: terminatord [hkmwd]" << endl << endl;
    cout << "Terminator daemon is killing processes that exceed some limit" << endl << endl;
    cout << "Arguments:" << endl;
    cout << " -h [--help]: Display this help" << endl;
    cout << " -k [--kill]: Will kill processes if they exceed the limits" << endl;
    cout << " -m [limit in MB]: Set a memory limit" << endl;
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
        cout << "Usage: terminatord [hkmwd]" << endl;
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

                }
                c++;
            }
            continue;
        }
    }
    return false;
}


