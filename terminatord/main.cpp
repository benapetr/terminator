#include <iostream>

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

    return 0;
}
