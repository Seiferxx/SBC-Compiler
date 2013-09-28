#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"
#include "Token.h"

#include <iostream>
using std::cout;
using std::endl;

SyntaxAnalyzer::SyntaxAnalyzer( LexAnalyzer& l ) :
    lexAnalyzer( l ),
    curToken( 0 ),
    correctSyntax_( true ){
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    if( curToken != 0 ){
        delete curToken;
    }
}

bool SyntaxAnalyzer::correctSyntax(){
    return correctSyntax_;
}

bool SyntaxAnalyzer::analyze(){
    consumeToken();
    while( curToken != 0 and correctSyntax_ ){
        global();
    }
    
    return correctSyntax_;
}

void SyntaxAnalyzer::consumeToken(){
    if( curToken != 0 ){
        delete curToken;
        curToken = 0;
    }
    curToken = lexAnalyzer.getToken();
    if( curToken != 0 ){
        cout << curToken -> getTypeString() << endl;
    }
}

void SyntaxAnalyzer::check( int t ){
    if( curToken -> type() == t ){
        consumeToken();
    }
    else{
        raiseError( "Syntax Error" );
    }
}

void SyntaxAnalyzer::raiseError( const char* except ){
    throw except;
}

void SyntaxAnalyzer::global(){
    int t = curToken -> type();
    
    // typedef rule
    if( t == Token::TYPEDEF_K ){
        consumeToken();
        type();
        check( Token::IDENTIFIER );
        check( Token::SEMI_COLON );
    }
    // type declaration rule
    else if( t == Token::STRUCT_K || t == Token::UNION_K ){
        consumeToken();
        check( Token::IDENTIFIER );
        check( Token::LEFT_CB );
        declarationList();
        check( Token::RIGHT_CB );
        check( Token::SEMI_COLON );
    }
    // function rule
    else{
        raiseError( "Rule not implmented yet." );
    }
}

void SyntaxAnalyzer::type(){
    int t = curToken -> type();
    
    if( t == Token::INT_K || t == Token::LONG_K || t == Token::CHAR_K ||
        t == Token::FLOAT_K || t == Token::BOOL_K ){
        consumeToken();
    }
    // user defined type rule
    else if( t == Token::STRUCT_K || t == Token::UNION_K ){
        consumeToken();
        check( Token::IDENTIFIER );
    }
}

void SyntaxAnalyzer::declarationList(){
    varDeclaration();
    check( Token::SEMI_COLON );
    if( curToken -> type() != Token::RIGHT_CB ){
        declarationList();
    }
}

void SyntaxAnalyzer::varDeclaration(){
    type();
    check( Token::IDENTIFIER );
}
