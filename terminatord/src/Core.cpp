#include "../include/Configuration.h"
#include "../include/Core.h"

string Core::GetCurrentTime()
{
    // Print a current time
    time_t t = time(0);
    struct tm * now = localtime( & t );
    ostringstream s;
    s << now->tm_mday << " " << (now->tm_mon + 1) << " " << (now->tm_year + 1900) << " " << now->tm_hour
    << ":" << now->tm_min << ":" << now->tm_sec;
    return s.str();
}

void Core::Log(string text)
{


    cout << "[" << GetCurrentTime() << "] " << text << endl;
}

void Core::DebugLog(string text, unsigned int verbosity)
{
    if (verbosity >= Configuration::Verbosity)
    {
        Log("DEBUG: " + text);
    }
}
