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
    
    public:
        explicit SyntaxAnalyzer( LexAnalyzer& l );
        ~SyntaxAnalyzer();
        bool correctSyntax();
        bool analyze();
        void consumeToken();
        void check( int t );
};

#endif //SYNTAX_ANALYZER_H
