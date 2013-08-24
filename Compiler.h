#ifndef SBC_COMPILER_H
#define SBC_COMPILER_H

#include <vector>
using std::vector;

#include <string>
using std::string;

class sbcCompiler{
    private:
        fstream srcFile;
        vector<string> lines;
        
        void readFile();
    public:
        sbcCompiler( const char* filename );
        ~sbcCompiler();
};
