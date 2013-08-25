#include "LexAnalyzer.h"
#include "Token.h"
#include "Compiler.h"

#include <cctype>
using std::isdigit;
using std::isalpha;

#include <algorithm>
using std::binary_search;

#define ACCEPTED_LEN 25

// Constructor method
LexAnalyzer::LexAnalyzer( sbcCompiler& c ) :
    line(),
    curLine_( 0 ),
    curChar_( 0 ),
    compiler( c ){
}

// Destructor method
LexAnalyzer::~LexAnalyzer(){
}

// Accessor to member curLine_
int LexAnalyzer::curLine() const{
    return curLine_;
}

// Accessor to member curChar_
int LexAnalyzer::curChar() const{
    return curChar_;
}

// Gets next token from the file's lines.
Token* LexAnalyzer::getToken(){
    Token* token = 0;
    bool moreTokens = true;
    string symbol;
    
    if( line.empty() ){
        line = compiler.getLine( 0 );
        curLine_ = 0;
        curChar_ = 0;
    }
    else if( curChar_ >= line.size() ){
        if( not getNextLine() ){
            moreTokens = false;
        }
    }
    else if( not findTokenStart() ){
        moreTokens = false;
    }
    
    if( moreTokens ){
        symbol = getSymbol();
        token = new Token( symbol, getType( curState ) );
    }
    
    return token;
}

// Checks if c is a character that ends a token.
bool LexAnalyzer::endOfToken( char c ){
    return c == '\n' or c == '\r' or c == ' ' or c == '\t';
}

// Checks if single char can be part of an escape sequence.
bool LexAnalyzer::isEscSequence( char c ){
    return c == '\'' or c == '\"' or c == '?' or c == '\\' or
           c == 'a' or c == 'b' or c == 'f' or c == 'n' or
           c == 'r' or c == 't' or c == 'v' or c == '0';
}

// Checks if the given char is a valid token of lenght 1.
bool LexAnalyzer::isSingleChar( char c ){
    return c == '{' or c == '}' or c == '(' or c == ')' or
           c == ';' or c == '#' or c == '[' or c == ']';
}

int LexAnalyzer::getSingleCharState( char c ){
    char characters[] = {'{', '}', '(', ')', ';', '#', '[', ']' };
    int i;
    
    for( i = 0; i < 8; i++ ){
        if( c == characters[i] ){
            return i;
        }
    }
}

bool LexAnalyzer::getNextLine(){
    bool success = false;
    
    if( curLine_ + 1 < compiler.numLines() ){
        curLine_++;
        line = compiler.getLine( curLine_ );
        curChar_ = 0;
        success = true;
    }
    
    return success;
}

// Finds the start of the next token.
bool LexAnalyzer::findTokenStart(){
    bool goOn = true;
    
    while( endOfToken( line.at( curChar_ ) ) and goOn ){
        curChar_++;
        if( curChar_ >= line.size() ){
            if( not getNextLine() ){
                goOn = false;
            }
        }
    }
    
    return goOn;
}

// Gets token string.
string LexAnalyzer::getSymbol(){
    string symbol;
    bool done = false;
    bool error = false;
    
    curState = 0;
    if( not isSingleChar( line[curChar_] ) ){
        while( not done and not error ){
            symbol.push_back( line[curChar_] );
            curState = nextState( curState, line[curChar_] );
            if( curState >= 0 ){
                if( acceptState( curState ) ){
                    if( not includeNextChar( curState ) ){
                        done = true;
                    }
                }
            }
            else{
                error = true;
            }
            curChar_++;
        }
        
        if( error ){
            while( not endOfToken( line[curChar_] ) and curChar_ < line.size() ){
                symbol.push_back( line[curChar_] );
                curChar_++;
            }
        }
    }
    else{
        symbol.push_back( line[curChar_] );
        curState = STATES + getSingleCharState( line[curChar_] );
        curChar_++;
    }
    
    return symbol;
}

// Gets the next state according to the transitions matrix.
int LexAnalyzer::nextState( int state, char input ){
                                    //  L,   D,  E,  .,  '   ",  \,  C, ES,  +,  -,  *,  /,  %,  >,  <,  !,  &,  |,  =,  @
    int transitions[STATES][INPUTS] = {  1,  2,  1,  3,  8, 13, -1, -1, -1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                                         1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  2,  5,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  7, -1, -1, -1, -1, -1, -1, -1,  6,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                         9,  9,  9,  9, -1, -1, 10,  9, -1,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
                                        -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        13, 13, 13, 13, -1, 15, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
                                        -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, 30, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,- 1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1,- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 34, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,};
    int nextState = -1;
    
    if( state != 11 or state == 14 ){
        switch( input ){
        case 'e': case 'E':
            nextState = transitions[state][EXP];
            break;
        case '.':
            nextState = transitions[state][POINT];
            break;
        case '\'':
            nextState = transitions[state][S_QOUTE];
            break;
        case '\"':
            nextState = transitions[state][D_QOUTE];
            break;
        case '\\':
            nextState = transitions[state][BACKSLASH];
            break;
        case '+':
            nextState = transitions[state][PLUS];
            break;
        case '-':
            nextState = transitions[state][MINUS];
            break;
        case '*':
            nextState = transitions[state][PROD];
            break;
        case '/':
            nextState = transitions[state][DIV];
            break;
        case '%':
            nextState = transitions[state][MOD];
            break;
        case '>':
            nextState = transitions[state][GREATER];
            break;
        case '<':
            nextState = transitions[state][LESSER];
            break;
        case '!':
            nextState = transitions[state][NOT];
            break;
        case '&':
            nextState = transitions[state][AND];
            break;
        case '|':
            nextState = transitions[state][OR];
            break;
        case '=':
            nextState = transitions[state][EQ];
            break;
        case '@':
            nextState = transitions[state][AT];
            break;
        default:
            if( isalpha( input ) || input == '_' ){
                nextState = transitions[state][LETTER];
            }
            else if( isdigit( input ) ){
                nextState = transitions[state][DIGIT];
            }
            else{
                nextState = transitions[state][CHAR];
            }
            break;
        }
    }
    else if( isEscSequence( input ) ){
        nextState = transitions[state][ESC];
    }
    
    return nextState;
}

// Checks if a given state is a terminal state.
bool LexAnalyzer::acceptState( int state ){
    int states[ACCEPTED_LEN] = {1, 2, 4, 7, 10, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                                26, 27, 28, 29, 30, 31, 32, 33, 34 };
                    
    return binary_search( states, states + ACCEPTED_LEN, state );
}

// Checks if the next character can be included in the current token.
bool LexAnalyzer::includeNextChar( int state ){
    int next;
    int include = false;
    
    if( curChar_ + 1 < line.size() ){
        next = nextState( curState, line[curChar_ + 1] );
        include = ( next >= 0 and not endOfToken( line[curChar_ + 1] ) );
    }
    
    return include;
}

// Gets the token type of the specified state.
int LexAnalyzer::getType( int state ){
    int type = -1;
    
    switch( state ){
    case 1:
        type = Token::IDENTIFIER;
        break;
    case 2:
        type = Token::INTEGER;
        break;
    case 4: case 7:
        type = Token::FLOAT;
        break;
    case 10:
        type = Token::CHAR;
        break;
    case 15:
        type = Token::STRING;
        break;
    case 16:
        type = Token::PLUS;
        break;
    case 17:
        type = Token::MINUS;
        break;
    case 18:
        type = Token::PROD;
        break;
    case 19:
        type = Token::DIV;
        break;
    case 20:
        type = Token::MOD;
        break;
    case 21:
        type = Token::GREATER;
        break;
    case 22:
        type = Token::LESSER;
        break;
    case 23:
        type = Token::NOT;
        break;
    case 24:
        type = Token::AND;
        break;
    case 25:
        type = Token::OR;
        break;
    case 26:
        type = Token::ASSIGN;
        break;
    case 27:
        type = Token::AT;
        break;
    case 28:
        type = Token::INCR;
        break;
    case 29:
        type = Token::DECR;
        break;
    case 30:
        type = Token::AT_P;
        break;
    case 31:
        type = Token::GREATER_EQ;
        break;
    case 32:
        type = Token::LESSER_EQ;
        break;
    case 33:
        type = Token::NOT_EQ;
        break;
    case 34:
        type = Token::EQUALS;
        break;
    case 35:
        type = Token::LEFT_CB;
        break;
    case 36:
        type = Token::RIGHT_CB;
        break;
    case 37:
        type = Token::LEFT_P;
        break;
    case 38:
        type = Token::RIGHT_P;
        break;
    case 39:
        type = Token::SEMI_COLON;
        break;
    case 40:
        type = Token::HASH;
        break;
    case 41:
        type = Token::LEFT_BRKT;
        break;
    case 42:
        type = Token::RIGHT_BRKT;
        break;
    }
    
    return type;
}
