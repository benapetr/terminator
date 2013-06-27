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

using namespace std;

class Parser
{
    public:
        Parser(int c, char *v[]);
        bool Parse();
    protected:
    private:
        bool is_number(const std::string& s);
        int argc;
        char **argv;
        void ShowHelp();
};

#endif // PARSER_H
