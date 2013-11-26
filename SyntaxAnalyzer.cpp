#include "SyntaxAnalyzer.h"
#include "LexAnalyzer.h"
#include "Token.h"

#include <iostream>
using std::cout;
using std::endl;

//Revisar declaracion de uniones/estructuras.

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
        cout << curToken -> getTypeString()
             << " " << curToken -> symbol() << endl;
    }
}

void SyntaxAnalyzer::check( int t ){
    if( correctSyntax_ ){
        if( curToken != 0 && curToken -> type() == t ){
            consumeToken();
        }
        else{
            raiseError( "Syntax Error" );
        }
    }
}

void SyntaxAnalyzer::raiseError( const char* except ){
    cout << "Token: " << curToken -> symbol() << endl;
    correctSyntax_ = false;
    //throw except;
}

void SyntaxAnalyzer::global(){
    int t;
    
    if( curToken != 0 ){
        t = curToken -> type();
        // typedef rule --- GONE
        /*if( t == Token::TYPEDEF_K ){
            consumeToken();
            type();
            check( Token::IDENTIFIER );
            check( Token::SEMI_COLON );
        }*/
        // type declaration rule
        if( t == Token::STRUCT_K || t == Token::UNION_K ){
            consumeToken();
            check( Token::IDENTIFIER );
            check( Token::LEFT_CB );
            declarationList();
            check( Token::RIGHT_CB );
            check( Token::SEMI_COLON );
        }
        // function rule
        else if( t == Token::VOID_K ){
            consumeToken();
            check( Token::IDENTIFIER );
            check( Token::LEFT_P );
            params();
            check( Token::RIGHT_P );
            block();
        }
        else{
            type();
            check( Token::IDENTIFIER );
            check( Token::LEFT_P );
            params();
            check( Token::RIGHT_P );
            fblock();
        }
    }
}

void SyntaxAnalyzer::type(){
    int t;
    
    if( curToken != 0 ){
        t = curToken -> type();
        
        if( t == Token::INT_K || t == Token::LONG_K || t == Token::CHAR_K ||
            t == Token::FLOAT_K || t == Token::BOOL_K ){
            consumeToken();
        }
        // user defined type rule
        else if( t == Token::STRUCT_K || t == Token::UNION_K ){
            consumeToken();
            check( Token::IDENTIFIER );
        }
        else{
            raiseError( "Syntax Error" );
        }
    }
}

void SyntaxAnalyzer::declarationList(){
    if( curToken != 0 ){
        varDeclaration();
        check( Token::SEMI_COLON );
        if( curToken != 0 && curToken -> type() != Token::RIGHT_CB ){
            declarationList();
        }
    }
}

void SyntaxAnalyzer::varDeclaration(){
    type();
    check( Token::IDENTIFIER );
}

void SyntaxAnalyzer::params(){
    if( curToken != 0 && curToken -> type() != Token::VOID_K ){
        paramList();
    }
    else{
        consumeToken();
    }
}

void SyntaxAnalyzer::paramList(){
    varDeclaration();
    if( curToken != 0 && curToken -> type() == Token::COMMA ){
        consumeToken();
        paramList();
    }
}

void SyntaxAnalyzer::fblock(){
    check( Token::LEFT_CB );
    while( curToken != 0 && curToken -> type() != Token::RETURN_K ){
        instruction();
    }
    check( Token::RETURN_K );
    assignment();
    check( Token::SEMI_COLON );
    check( Token::RIGHT_CB );
}

void SyntaxAnalyzer::block(){
    check( Token::LEFT_CB );
    while( curToken != 0 && curToken -> type() != Token::RIGHT_CB ){
        instruction();
    }
    check( Token::RIGHT_CB );
}

void SyntaxAnalyzer::instruction(){
    int t;
    
    if( curToken != 0 ){
        t = curToken -> type();
        
        if( t == Token::LEFT_CB ){
            block();
        }
        else if( t == Token::IF_K || t == Token::WHILE_K || t == Token::FOR_K ){
            control();
        }
        // local variable declaration rule
        else if( t == Token::INT_K || t == Token::LONG_K || t == Token::CHAR_K ||
                 t == Token::FLOAT_K || t == Token::BOOL_K || t == Token::STRUCT_K ||
                 t == Token::UNION_K ){
            varDeclaration();
            check( Token::SEMI_COLON );
        }
        else{
            assignment();
            check( Token::SEMI_COLON );
        }
    }
}

void SyntaxAnalyzer::control(){
    int t;
    
    if( curToken != 0 ){
        t = curToken -> type();
    
        // if rule
        if( t == Token::IF_K ){
            consumeToken();
            check( Token::LEFT_P );
            assignment();
            check( Token::RIGHT_P );
            block();
            if( curToken != 0 && curToken -> type() == Token::ELSE_K ){
                consumeToken();
                block();
            }
        }
        // while rule
        else if( t == Token::WHILE_K ){
            consumeToken();
            check( Token::LEFT_P );
            assignment();
            check( Token::RIGHT_P );
            block();
        }
        // for rule
        else if( t == Token::FOR_K ){
            consumeToken();
            check( Token::LEFT_P );
            assignment();
            check( Token::SEMI_COLON );
            assignment();
            check( Token::SEMI_COLON );
            assignment();
            check( Token::RIGHT_P );
            block();
        }
    }
}

void SyntaxAnalyzer::assignment(){
    logicOr();
    while( curToken != 0 && curToken -> type() == Token::ASSIGN ){
        consumeToken();
        assignment();
    }
}

void SyntaxAnalyzer::logicOr(){
    logicAnd();
    while( curToken != 0 && curToken -> type() == Token::OR ){
        consumeToken();
        logicOr();
    }
}

void SyntaxAnalyzer::logicAnd(){
    equals();
    while( curToken != 0 && curToken -> type() == Token::AND ){
        consumeToken();
        logicAnd();
    }
}

void SyntaxAnalyzer::equals(){
    relational();
    while( curToken != 0 && ( curToken -> type() == Token::EQUALS || curToken -> type() == Token::NOT_EQ ) ){
        consumeToken();
        equals();
    }
}

void SyntaxAnalyzer::relational(){
    sum();
    while( curToken != 0 && ( curToken -> type() == Token::GREATER || curToken -> type() == Token::LESSER ||
           curToken -> type() == Token::GREATER_EQ || curToken -> type() == Token::LESSER_EQ ) ){
        consumeToken();
        relational();
    }
}

void SyntaxAnalyzer::sum(){
    mult();
    while( curToken != 0 && ( curToken -> type() == Token::PLUS || curToken -> type() == Token::MINUS ) ){
        consumeToken();
        sum();
    }
}

void SyntaxAnalyzer::mult(){
    logicNot();    
    while( curToken != 0 && ( curToken -> type() == Token::PROD || curToken -> type() == Token::DIV ||
           curToken -> type() == Token::MOD ) ){
        consumeToken();
        mult();
    }
}

void SyntaxAnalyzer::logicNot(){
    if( curToken != 0 && curToken -> type() == Token::NOT ){
        consumeToken();
        logicNot();
    }
    else{
        sign();
    }
}

void SyntaxAnalyzer::sign(){
    if( curToken != 0 && ( curToken -> type() == Token::PLUS || curToken -> type() == Token::MINUS ) ){
        consumeToken();
        sign();
    }
    else{
        postfix();
    }
}

void SyntaxAnalyzer::postfix(){
    value();
    if( curToken != 0 && curToken -> type() == Token::INCR || curToken -> type() == Token::DECR ){
        consumeToken();
    }
    else if( curToken != 0 && curToken -> type() == Token::LEFT_P ){
        consumeToken();
        if( curToken != 0 && curToken -> type() != Token::RIGHT_P ){
            args();
        }
        check( Token::RIGHT_P );
    }
    /*
    else if( curToken -> type() == Token::LEFT_BRKT ){
        consumeToken();
        assignment();
        check( Token::RIGHT_BRKT );
    }
    */
    else if( curToken != 0 && curToken -> type() == Token::AT ){
        consumeToken();
        check( Token::IDENTIFIER );
    }
}

void SyntaxAnalyzer::value(){
    if( curToken != 0 && curToken -> type() == Token::LEFT_P ){
        consumeToken();
        assignment();
        check( Token::RIGHT_P );
    }
    else if( curToken != 0 && curToken -> type() == Token::IDENTIFIER ){
        check( Token::IDENTIFIER );
    }
    else{
        constant();
    }
}

void SyntaxAnalyzer::args(){
    assignment();
    while( curToken != 0 && curToken -> type() == Token::COMMA ){
        consumeToken();
        args();
    }
}

void SyntaxAnalyzer::constant(){
    int t = curToken -> type();
    
    if( curToken != 0 && ( t == Token::INT_C || t == Token::FLOAT_C || t == Token::CHAR_C || t == Token::STRING_C ||
        t == Token::TRUE_K || t == Token::FALSE_K ) ){
        consumeToken();
    }
}
