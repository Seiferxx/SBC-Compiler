#include "Token.h"

#define KEYWORDS 17

Token::Token() : symbol_( "" ), type_( -1 ){
}

Token::Token( const string& symbol, int type ) :
    symbol_( symbol ),
    type_( type ){
}

Token::Token( const Token& t ) : 
    symbol_( t.symbol() ),
    type_( t.type() ){
}

Token::~Token(){
    symbol_.clear();
}

string Token::symbol() const{
    return symbol_;
}

int Token::type() const{
    return type_;
}

void Token::setSymbol( const string& symbol ){
    symbol_ = symbol;
}

void Token::setType( int type ){
    type_ = type;
}

const char* Token::getTypeString(){
    const char* types[] = { "Identifier", "Int_c", "Float_c", "Char_c", "String literal", "+", "-", "*", "/", "%",
                            ">", "<", "!", "&", "|", "=", "@", "++", "--", "->", ">=", "<=", "!=", "==", "{",
                            "}", "(", ")", ";", "#", "[", "]", ",", "bool_k", "break_k", "char_k", "else_k",
                            "false_k", "float_k", "for_k", "if_k", "int_k", "long_k", "return_k",
                            "struct_k", "true_k", "typedef_k", "union_k", "void_k", "while_k" };
    if( type_ >= 0 ){
        return types[type_];
    }
    
    return "error";
}

void Token::checkIfKeyword(){
    const char* keywords[] = { "bool", "break", "char", "else", "false", "float",
                               "for", "if", "int", "long", "return", "struct", "true",
                               "typedef", "union", "void", "while" };                 
    int i = 0;
    int j = KEYWORDS - 1;
    int center;
    int comparison;
    bool found = false;
    
    while( i <= j and not found ){
        center = ( i + j ) / 2;
        comparison = symbol_.compare( keywords[center] );
        if( comparison < 0 ){
            j = center - 1;
        }
        else if( comparison > 0 ){
            i = center + 1;
        }
        else{
            type_ = TOKEN_TYPES + center;
            found = true;
        }
    }
}
