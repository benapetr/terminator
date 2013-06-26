#include <unistd.h>
#include "../include/Core.h"
#include "../include/Configuration.h"
#include "../include/Watcher.h"


bool Watcher::Running = false;

void Watcher::Start()
{
    Running = true;
    while (Running)
    {
        usleep(Configuration::Interval);
    }
}
