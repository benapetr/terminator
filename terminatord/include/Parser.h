 // This program is free software; you can redistribute it and/or modify
 // it under the terms of the GNU General Public License as published by
 // the Free Software Foundation; either version 2 of the License, or
 // (at your option) version 3.

 // This program is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 // GNU General Public License for more details.

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "Configuration.h"
#include "Writer.h"
#include "Core.h"

using namespace std;

namespace terminator
{
    class Parser
    {
        public:
            static bool IsNumber(const std::string& s);
            Parser(int c, char *v[]);
            bool Parse();
        private:
            void ShowHelp();
            bool NextIsNumber(int next);
            int argc;
            char **argv;
    };
}

#endif // PARSER_H
