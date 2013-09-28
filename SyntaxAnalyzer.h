#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

class LexAnalyzer;
class Token;

class SyntaxAnalyzer{
    private:
        LexAnalyzer& lexAnalyzer;
        Token* curToken;
        bool correctSyntax_;
    
        void raiseError( const char* except );
        void global();
        void type();
        void declarationList();
        void varDeclaration();
        void params();
        void paramList();
        void block();
        void instruction();
        void control();
        void assignment();
        void logicOr();
        void logicAnd();
        void equals();
        void relational();
        void sum();
        void mult();
        void logicNot();
        void sign();
        void postfix();
        void value();
        void args();
        void constant();
    
    public:
        explicit SyntaxAnalyzer( LexAnalyzer& l );
        ~SyntaxAnalyzer();
        bool correctSyntax();
        bool analyze();
        void consumeToken();
        void check( int t );
};

#endif //SYNTAX_ANALYZER_H
