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
        int argc;
        char **argv;
        void ShowHelp();
};

#endif // PARSER_H
