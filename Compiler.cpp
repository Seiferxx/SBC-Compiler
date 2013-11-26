#include "Compiler.h"
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
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
    syntaxAnalyzer = new SyntaxAnalyzer( *lexAnalyzer );
}

// Destructor method.
sbcCompiler::~sbcCompiler(){
    if( srcFile.is_open() ){
        srcFile.close();
    }
    delete lexAnalyzer;
    delete syntaxAnalyzer;
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
    bool result;
    
    readFile();
    try{
        result = syntaxAnalyzer -> analyze();
        if( result ){
            cout << "Ok" << endl;
        }
        else{
            cout << "Syntax error." << endl;
        }
    }
    catch( const char* except ){
        cout << except << " in line " << lexAnalyzer -> curLine() + 1
             << ":" << lexAnalyzer -> curChar() + 1 << endl;
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
