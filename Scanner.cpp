#include "Scanner.h"
#include "Logging.h"
#include <map>


std::map<std::string, TokenType> KeywordMap
{
    {"if", TokenType::IF},
    {"var", TokenType::VAR},
    {"class", TokenType::CLASS},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"print", TokenType::PRINT},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"or", TokenType::OR},
    {"and", TokenType::AND},
    {"fun", TokenType::FUN},
    {"naww", TokenType::NAWW},

};

void Scanner::Comment()
{
    if(Peek() == '#')
    //if(Peek() == '/' && PeekNext() == '/')
    {
        while(Peek() != '\n' && Peek() != '\0')
        {
            Advance();
        }

        Log("COMMENT FOUND");
    }
}

char& Scanner::Peek()
{
    return *Current;
}

char Scanner::PeekNext()
{
    char Next = *++Current;
    --Current;
    return Next;
}

const char& Scanner::Advance()
{
    CharBuffer += Peek();
    Current++;
    return Current[-1];
}

void Scanner::SkipWhitespace()
{
    for(;;)
    {
        switch(*Current)
        {
            case '\n': Line++; Advance(); break;
            case '\r':
            case '\t':
            case ' ' : Advance(); break;

            default: return;
        };
    }
}

bool IsAlpha(const char& Char)
{
    return (Char >= 'a' && Char <= 'z') ||
           (Char >= 'A' && Char <= 'Z') ||
            Char == '_';
}

bool IsDigit(const char& Char)
{
    return Char >= '0' && Char <= '9';
}

Token Scanner::MakeToken(TokenType TokenType)
{
    return Token(Line, CharBuffer, TokenType);
}

bool Scanner::Match(char Expected)
{
    if(Expected == Peek())
    {
        Advance();
        return true;
    }

    return false;
}

Token Scanner::Number()
{
    while(IsDigit(Peek()))
    {
        Advance();
    }

    Logf("NUMBER FOUND {}", CharBuffer);
    return MakeToken(TokenType::NUMBER); 
}

Token Scanner::Identifier()
{
    while(IsAlpha(Peek()) || IsDigit(Peek()))
    {
        Advance();
    }

    if(KeywordMap.contains(CharBuffer))
    {
        Logf("KEYWORD FOUND {}", CharBuffer);
        return MakeToken(KeywordMap[CharBuffer]);
    }
    Logf("IDENTIFIER FOUND {}", CharBuffer);
    return MakeToken(TokenType::IDENTIFIER);
}

Token Scanner::String()
{
    Advance();
    while(Peek() != '"')
    {
        // TODO: Add compile errors for this when we have compile errors
        if(Peek() == '\0')
        {
            return MakeToken(TokenType::STRING);
        }

        Advance();
    }

    Current++;

    Logf("STRING FOUND {}", CharBuffer);
    return MakeToken(TokenType::STRING);
}

Token Scanner::ScanToken()
{
    const char& Char = *Current;

    switch(Char)
    {
    case '<': Advance(); return Match('=') ?
                             MakeToken(TokenType::GREATER_EQUAL) : MakeToken(TokenType::GREATER);
    case '>': Advance(); return Match('=') ?
                             MakeToken(TokenType::LESS_EQUAL) : MakeToken(TokenType::LESS);
    case '!': Advance(); return Match('=') ?
                             MakeToken(TokenType::BANG_EQUAL) :MakeToken(TokenType::BANG);
    case '=': Advance(); return Match('=') ?
                             MakeToken(TokenType::EQUAL_EQUAL) : MakeToken(TokenType::EQUAL);
        case ';' : Advance(); return MakeToken(TokenType::SEMICOLON);
        case '+' : Advance(); return MakeToken(TokenType::PLUS);
        case '-' : Advance(); return MakeToken(TokenType::MINUS);
        case '*' : Advance(); return MakeToken(TokenType::STAR);
        case '/' : Advance(); return MakeToken(TokenType::SLASH);
        case '(' : Advance(); return MakeToken(TokenType::LEFT_PAREN);
        case ')' : Advance();  return MakeToken(TokenType::RIGHT_PAREN);
        case '{' : Advance(); return MakeToken(TokenType::LEFT_BRACE);
        case '}' : Advance(); return MakeToken(TokenType::RIGHT_BRACE);
        case '"' : Current++; return String();

        default: break;
    }

    if(IsDigit(Char))
    {
        return Number();
    }

    // Currently identifiers can't start with digits.
    // Sue me.
    if(IsAlpha(Char))
    {
        return Identifier();
    }
    
    return MakeToken(TokenType::ENDOFFILE);
}

std::vector<Token> Scanner::ScanTokens(std::string Source)
{
    Current = &Source[0];
    std::vector<Token> Tokens;
    while(Current && *Current != '\0')
    {
        Comment();
        SkipWhitespace();

        CharBuffer = "";
        Token NewToken = ScanToken();
        Tokens.push_back(NewToken);

        Previous = *Current;
    }

    Tokens.push_back(MakeToken(TokenType::ENDOFFILE));
    Log("Reached end");
    
    return Tokens;
}
