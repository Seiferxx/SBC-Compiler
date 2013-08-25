#ifndef LEX_ANALIZER_H
#define LEX_ANALIZER_H


#include <string>
using std::string;

#define INPUTS 21
#define STATES 35

class Token;
class sbcCompiler;

class LexAnalyzer{
    private:
        enum InputType{
            LETTER = 0, DIGIT, EXP, POINT,
            S_QOUTE, D_QOUTE, BACKSLASH, CHAR, ESC,
            PLUS, MINUS, PROD, DIV, MOD,
            GREATER, LESSER, NOT, AND, OR,
            EQ, AT
        };
        
        sbcCompiler& compiler;
        string line;
        int curLine_;
        int curChar_;
        int curState;
        
        inline bool endOfToken( char c );
        inline bool isEscSequence( char c );
        inline bool isSingleChar( char c );
        int getSingleCharState( char c );
        bool getNextLine();
        bool findTokenStart();
        string getSymbol();
        int nextState( int state, char input );
        bool acceptState( int state );
        bool includeNextChar( int state );
        int getType( int state );
        
    public:
        LexAnalyzer( sbcCompiler& c );
        ~LexAnalyzer();
        int curLine() const;
        int curChar() const;
        Token* getToken();
};

#endif //LEX_ANALIZER_H
