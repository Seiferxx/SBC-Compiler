#ifndef SBC_COMPILER_H
#define SBC_COMPILER_H

#include <fstream>
using std::fstream;

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

#endif //SBC_COMPILER_H
