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
        static void ErrorLog(string text);
        static string int2String(int number);
        static string Long2String(unsigned long number);
    protected:
    private:
        static string GetCurrentTime();
        static string DecimalNumber(int n);
};

#endif // CORE_H
