#include "Token.h"
#include <vector>

class Scanner
{
 public:
    char* Current;
    int Line = 0;
    std::string CharBuffer;
    std::vector<Token> ScanTokens(std::string Source);
    Token ScanToken();

 private:
    char& Peek();
    char Previous;
    char PeekNext();
    const char& Advance();
    void SkipWhitespace();
    void Comment();
    bool Match(char Type);

    Token Number();
    Token String();
    Token Identifier();

    Token MakeToken(TokenType TokenType);
};
