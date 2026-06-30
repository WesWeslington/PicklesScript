#include "Scanner.h"
#include "Logging.h"
#include <map>

std::map<std::string, TokenType> KeywordMap
{
    {"if", IF},
    {"var", VAR},
    {"class", CLASS},
    {"for", FOR},
    {"while", WHILE},
    {"print", PRINT},
    {"true", TRUE},
    {"false", FALSE},
    {"or", OR},
    {"and", AND},
    {"fun", FUN},
    {"naww", NAWW},

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
    ++Current;
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

Token Scanner::Number()
{
    while(IsDigit(Peek()))
    {
        CharBuffer += Peek();
        Advance();
    }

    Logf("NUMBER FOUND {}", CharBuffer);
    return MakeToken(NUMBER); 
}

Token Scanner::Identifier()
{
    while(IsAlpha(Peek()) || IsDigit(Peek()))
    {
        CharBuffer += Peek();
        Advance();
    }

    if(KeywordMap.contains(CharBuffer))
    {
        Logf("KEYWORD FOUND {}", CharBuffer);
        return MakeToken(KeywordMap[CharBuffer]);
    }
    Logf("IDENTIFIER FOUND {}", CharBuffer);
    return MakeToken(IDENTIFIER);
}

Token Scanner::String()
{
    Advance();
    while(Peek() != '\"')
    {
        // TODO: Add compile errors for this when we have compile errors
        if(Peek() == '\0')
        {
            return MakeToken(STRING);
        }

        CharBuffer += Peek();
        Advance();
    }

    Logf("STRING FOUND {}", CharBuffer);
    return MakeToken(STRING);
}

Token Scanner::ScanToken()
{
    SkipWhitespace();
    const char& Char = *Current;

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

    switch(Char)
    {
        case '<': return PeekNext() == '=' ?
                MakeToken(GREATER_EQUAL) : MakeToken(GREATER);
        case '>': return PeekNext() == '=' ?
            MakeToken(LESS_EQUAL) : MakeToken(LESS);
        case '!': return PeekNext() == '=' ?
            MakeToken(BANG_EQUAL) :MakeToken(BANG);
        case '=': return PeekNext() == '=' ?
            MakeToken(EQUAL_EQUAL) : MakeToken(EQUAL);
        case ';' : return MakeToken(SEMICOLON);
        case '+' : return MakeToken(PLUS);
        case '-' : return MakeToken(MINUS);
        case '*' : return MakeToken(STAR);
        case '/' : return MakeToken(SLASH);
        case '(' : return MakeToken(LEFT_PAREN);
        case ')' : return MakeToken(RIGHT_PAREN);
        case '{' : return MakeToken(LEFT_BRACE);
        case '}' : return MakeToken(RIGHT_BRACE);
        case '\"': return String();

        default: break;
    }

    return MakeToken(ENDOFFILE);
}

std::vector<Token> Scanner::ScanTokens(std::string Source)
{
    Current = &Source[0];
    std::vector<Token> Tokens;
    for(;;)
    {
        CharBuffer = {};
        Comment();
        if(Current && *Current == '\0') { break; }
        
        Tokens.push_back(ScanToken());
        Advance();
    }
    
    return Tokens;
}
