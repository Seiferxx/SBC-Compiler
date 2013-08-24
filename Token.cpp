#include "Token.h"

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
