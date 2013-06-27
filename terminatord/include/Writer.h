 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.




#ifndef WRITER_H
#define WRITER_H

#include <iostream>
#include <list>
#include <unistd.h>
#include <fstream>
#include <list>
#include <mutex>
#include <pthread.h>

using namespace std;

class Writer
{
    public:
        static void Load();
    protected:
    private:
        static void *Exec();
        static list<Item> DB;
        static pthread_t thread;
        static bool isRunning;
};

#endif // WRITER_H
