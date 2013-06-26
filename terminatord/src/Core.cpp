#include "../include/Configuration.h"
#include "../include/Core.h"

//! Convert a number to 00 format
//! @param n Number
string Core::DecimalNumber(int n)
{
    ostringstream s;
    if (n >= 0 && n < 10)
    {
        s << "0" << n;
        return s.str();
    }
    s << n;
    return s.str();
}

//! Retrieve a current date and time
string Core::GetCurrentTime()
{
    // Print a current time
    time_t t = time(0);
    struct tm * now = localtime( & t );
    ostringstream s;
    s << now->tm_mday << ". " << DecimalNumber(now->tm_mon + 1) << ". " << (now->tm_year + 1900) << " "
    << DecimalNumber(now->tm_hour) << ":" << DecimalNumber(now->tm_min)
    << ":" << DecimalNumber(now->tm_sec);
    return s.str();
}

//! Log
//! @param text Text to log
void Core::Log(string text)
{
    cout << "[" << GetCurrentTime() << "] " << text << endl;
}

//! Log
//! @param text Text to log
void Core::ErrorLog(string text)
{
    cerr << "[" << GetCurrentTime() << "] ERROR: " << text << endl;
}

//! Log
//! @param text Text to log
void Core::DebugLog(string text, unsigned int verbosity)
{
    if (verbosity >= Configuration::Verbosity)
    {
        Log("DEBUG: " + text);
    }
}
