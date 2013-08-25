#ifndef SBC_COMPILER_H
#define SBC_COMPILER_H

#include <fstream>
using std::fstream;

#include <vector>
using std::vector;

#include <string>
using std::string;

class LexAnalyzer;

class sbcCompiler{
    private:
        fstream srcFile;
        LexAnalyzer* lexAnalyzer;
        vector<string> lines;
        int numLines_;
        
        void readFile();
        
    public:
        sbcCompiler( const char* filename );
        ~sbcCompiler();
        int numLines() const;
        string& getLine( int index );
        void compile();
};

#endif //SBC_COMPILER_H
