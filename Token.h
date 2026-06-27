#include <cstdint>
#include <string_view>

enum TokenType : uint8_t
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, PLUS, MINUS, SEMICOLLON, SLASH, STAR, BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        IF, ELSE, IDENTIFIER, STRING, NUMBER, AND, OR, CLASS, ENUM, FALSE,
        FOR, FUN, NAWW, /* NAWW is the Null for PicklesScript */ PRINT, RETURN,
        SUPER, THIS, TRUE, VAR, WHILE, SWITCH, CASE, QUERY, COLON, ERROR
};

struct Token
{
    int Line;
    std::string_view Lexeme;
    TokenType TokenType;
};
