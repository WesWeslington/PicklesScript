#include <cstdint>
#include <string>

enum TokenType : uint8_t
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, PLUS, MINUS, SEMICOLON, SLASH, STAR, BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        IF, ELSE, IDENTIFIER, STRING, NUMBER, AND, OR, CLASS, ENUM, FALSE,
        FOR, FUN, NAWW, /* NAWW is the Null for PicklesScript */ PRINT, RETURN,
        SUPER, THIS, TRUE, VAR, WHILE, SWITCH, CASE, QUERY, COLON, ERROR, ENDOFFILE
};

struct Token
{
    int Line;
    TokenType Type;
    std::string Lexeme;
    
    Token(){}
    Token(int NewLine, std::string NewLexeme, TokenType NewType)
    {
        Lexeme = NewLexeme;
        Line = NewLine;
        Type = NewType;
    }
};
