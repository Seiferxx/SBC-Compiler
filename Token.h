#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

#define TOKEN_TYPES 32

class Token{
    private:
        string symbol_;
        int type_;
        
    public:
        enum TokenType{
            IDENTIFIER = 0,
            INT_C, FLOAT_C, CHAR_C, STRING_C,
            PLUS, MINUS, PROD, DIV, MOD,
            GREATER, LESSER,
            NOT, AND, OR,
            ASSIGN,
            AT,
            INCR, DECR,
            AT_P,
            GREATER_EQ, LESSER_EQ, NOT_EQ, EQUALS,
            LEFT_CB, RIGHT_CB, LEFT_P, RIGHT_P,
            SEMI_COLON, HASH,
            LEFT_BRKT, RIGHT_BRKT
        };
    
        Token();
        Token( const string& symbol, int type );
        Token( const Token& t );
        ~Token();
        string symbol() const;
        int type() const;
        void setSymbol( const string& symbol );
        void setType( int type );
        const char* getTypeString();
        void checkIfKeyword();
};

#endif //TOKEN_H
