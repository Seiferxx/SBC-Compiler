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
        int numLines_;
        
        void readFile();
        
    public:
        sbcCompiler( const char* filename );
        ~sbcCompiler();
        int numLines() const;
        string& getLine( int index );
};

#endif //SBC_COMPILER_H
