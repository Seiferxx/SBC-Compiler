#include "Compiler.h"

#include <iostream>

using namespace std;

#define LEX_TEST "Test.txt"
#define SYNTAX_TEST "Test2.txt"

int main(){
    sbcCompiler compiler( SYNTAX_TEST );
    
    compiler.compile();
    
    return 0;
}
