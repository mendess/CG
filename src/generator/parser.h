#ifndef __PARSER_H__
#define __PARSER_H__

#include "patch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

class Parser {
    private:
        int tessellation;
        string filename;
        string patchfile;
        vector<Patch*> patches;
        string lineNumberContent(int, string);
    public:
        Parser(string, string, int);
        void parsePatchFile();
};

#endif