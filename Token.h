#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

class Token{
    private:
        string symbol_;
        int type_;
        
    public:
        Token();
        Token( const string& symbol, int type );
        Token( const Token& t );
        ~Token();
        string symbol() const;
        int type() const;
        void setSymbol( const string& symbol );
        void setType( int type );
};

#endif //TOKEN_H
