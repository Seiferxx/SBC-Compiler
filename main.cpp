#include "Compiler.h"

#include <iostream>

using namespace std;

int main(){
    sbcCompiler compiler( "Test.txt" );
    
    compiler.compile();
    
    return 0;
}
