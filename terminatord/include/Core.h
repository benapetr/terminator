#ifndef CORE_H
#define CORE_H

#include <sstream>
#include <iostream>

using namespace std;

class Core
{
    public:
        static void Log(string text);
        static void DebugLog(string text, unsigned int verbosity = 1);
    protected:
    private:
        static string GetCurrentTime();
};

#endif // CORE_H
