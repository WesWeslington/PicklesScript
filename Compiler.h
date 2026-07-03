#include "Scanner.h"

#include "VM.h"

#include <any>
#include <string>
#include <iostream>
#include <vector>

enum ExpressionType
{
    UNSET, BINARY, UNARY,
};

class BytecodeEmitter;
class AstPrinter;
class Expr
{
public:
    Expr(){}
    virtual ExpressionType Type(){return UNSET;}
};

class Binary : public Expr
{
public:
    Binary(Expr A, Token O, Expr B);
    /* : */
    /*     LHS(A), */
    /*     Operator(O), */
    /*         RHS(B){Logf("A {} B", O.Lexeme);} */
        //{}
    
    Expr LHS;
    Token Operator;
    Expr RHS;

protected:
    virtual ExpressionType Type(){return BINARY;}
    //virtual void Visit() override;
};

class Unary : public Expr
{
public:
    
    Unary(Expr A, Token O):
        Body(A),
        Operator(O)
    {}

    Expr Body;
    Token Operator;

protected:
    virtual ExpressionType Type(){return UNARY;}
};

class Grouping : public Expr
{
public:
    Grouping(Expr NewExpression):
        Expression(NewExpression)
    {}

    Expr Expression;
};

class Literal : public Expr
{
public:
    Literal(std::any V):
        Value(V)
    {}

    std::any Value;
};

class Compiler
{

public:
    Compiler(std::vector<Token> InitTokens)
        :Tokens(InitTokens)
    {}
    
    std::vector<Token> Tokens;
    
    Program Compile();
    
    void Execute();
    
    const Token Consume(TokenType Type, String Message);
    Token Previous();
    Token Advance();
    bool AtEndOfFile();
    const Token Peek();
    bool Match(const TokenType A);

    int Current = 0;
    
private:

    Expr ExprExpression();
    Expr ExprEquality();
    Expr ExprComparison();
    Expr ExprTerm(); 
    Expr ExprFactor();
    Expr ExprUnary();
    Expr ExprLiteral();
    
};
