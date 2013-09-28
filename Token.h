#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

#define TOKEN_TYPES 33

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
            LEFT_BRKT, RIGHT_BRKT,
            COMMA,
            BOOL_K, BREAK_K, CHAR_K, ELSE_K, FALSE_K,
            FLOAT_K, FOR_K, IF_K, INT_K, LONG_K, RETURN_K,
            STRUCT_K, TRUE_K, TYPEDEF_K, UNION_K, VOID_K,
            WHILE_K
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
