#include "Compiler.h"

#include <string>
using std::getline;

// Constructor method.
sbcCompiler::sbcCompiler( const char* filename ) :
    numLines_( 0 ){
    srcFile.open( filename, fstream::in );
}

// Destructor method.
sbcCompiler::~sbcCompiler(){
    if( srcFile.is_open() ){
        srcFile.close();
    }
}

// Gets number of lines in vector;
int sbcCompiler::numLines() const{
    return numLines_;
}

// Gets line from vector.
string& sbcCompiler::getLine( int index ){
    return lines.at( index );
}

// Reads and stores all lines in the file.
void sbcCompiler::readFile(){
    string line;
    
    getline( srcFile, line );
    while( not srcFile.eof() ){
        lines.push_back( line );
        getline( srcFile, line );
    }
    numLines_ = lines.size();
}
