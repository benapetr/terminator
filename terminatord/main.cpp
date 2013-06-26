#include <iostream>

#include "include/Configuration.h"
#include "include/Parser.h"
#include "include/Core.h"

using namespace std;

int main()
{
    Core::Log("Terminatord v" + Configuration::Version);
    return 0;
}
