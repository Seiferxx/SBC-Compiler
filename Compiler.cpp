#include "Compiler.h"
#include "LexAnalyzer.h"
#include "Token.h"

#include <string>
using std::getline;

#include <iostream>
using std::cout;
using std::endl;

// Constructor method.
sbcCompiler::sbcCompiler( const char* filename ) :
    numLines_( 0 ){
    
    srcFile.open( filename, fstream::in );
    lexAnalyzer = new LexAnalyzer( *this );
}

// Destructor method.
sbcCompiler::~sbcCompiler(){
    if( srcFile.is_open() ){
        srcFile.close();
    }
    delete lexAnalyzer;
}

// Gets number of lines in vector;
int sbcCompiler::numLines() const{
    return numLines_;
}

// Gets line from vector.
string& sbcCompiler::getLine( int index ){
    return lines.at( index );
}

// Main method. Compiles the source file.
void sbcCompiler::compile(){
    Token* token = 0;
    
    readFile();
    
    token = lexAnalyzer -> getToken();
    while( token != 0 ){
        cout << token -> symbol() << " " << token -> getTypeString() << endl;
        delete token;
        token = lexAnalyzer -> getToken();
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
    numLines_ = lines.size();
}
