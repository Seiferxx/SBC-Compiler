#include "Compiler.h"

#include <string>
using std::getline;

// Constructor method.
sbcCompiler::sbcCompiler( const char* filename ){
    srcFile.open( filename, fstream::in );
}

// Destructor method.
sbcCompiler::~sbcCompiler(){
    if( srcFile.is_open() ){
        srcFile.close();
    }
}

// Reads and stores all lines in the file.
void sbcCompiler::readFile(){
    string line;
    
    getline( srcFile, line );
    while( not srcFile.eof() ){
        lines.push_back( line );
        getline( srcFile, line );
    }
    
}
