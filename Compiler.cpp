#include "Compiler.h"
#include "Logging.h"

VM CurVM;

/*
  *    Expression
   */

Binary::Binary(Expr A, Token O, Expr B):
    LHS(A),
    Operator(O),
    RHS(B)
{
    Logf("A {} B", O.Lexeme);
}

Expr Compiler::ExprExpression()
{
    return ExprEquality();
}

Expr Compiler::ExprEquality()
{
    Expr _Expr = ExprComparison();
    while(Match(BANG_EQUAL) || Match(EQUAL_EQUAL))
    {
        Token Operator = Previous();
        Expr Right = ExprComparison();
        return Binary{Right, Operator, _Expr};
    }
    return _Expr;
}

Expr Compiler::ExprComparison()
{
    Expr _Expr = ExprTerm();

    while(Match(GREATER) || Match(GREATER_EQUAL) ||
          Match(LESS) || Match(LESS_EQUAL) )
    {
        Token Operator = Previous();
        Expr Right = ExprTerm();
        return Binary{Right, Operator, _Expr};
    }
    
    return _Expr;      
}

Expr Compiler::ExprTerm()
{
    Expr _Expr = ExprFactor();

    while(Match(PLUS) || Match(MINUS))
    {
        Token Operator = Previous();
        Expr Right = ExprFactor();
        return Binary{Right, Operator, _Expr};
    }

    return _Expr;
}

Expr Compiler::ExprFactor()
{
    Expr _Expr = ExprUnary();

    while(Match(STAR) || Match(SLASH))
    {
        Token Operator = Previous();
        Expr Right = ExprUnary();
        return Binary{Right, Operator, _Expr};
    }
        
    return _Expr;
}

Expr Compiler::ExprUnary()
{
    if(Match(MINUS) || Match(BANG))
    {
        Token Operator = Previous();
        Expr Right = ExprLiteral();
        return Unary{Right, Operator};
    }

    return ExprLiteral();
}

Expr Compiler::ExprLiteral()
{
    if(Match(FALSE)){ Log("False"); return Literal{false}; }
    if(Match(TRUE)) { Log("True");  return Literal{true};  }
    if(Match(NAWW)) { Log("Naww");  return Literal{NULL};  }

    if(Match(NUMBER))
    {
        int Num = std::stoi(Previous().Lexeme);
        return Literal{Num};
    }

    if(Match(STRING))
    {
        String LitString = Previous().Lexeme;
        return Literal{LitString};
    }

    if(Match(LEFT_PAREN))
    {
        Log("Before");
        Expr _Expr = ExprExpression();
        Log("After");
        Consume(RIGHT_PAREN, String("Expected ')' at the end of a grouping"));
        return Grouping{_Expr};
    }

    if(Match(ENDOFFILE))
    {
        return Literal{NULL};
    }

    Errf("Expected expression in literal, error at token type {}", Peek().Lexeme);
}

bool Compiler::AtEndOfFile()
{
    return Peek().Type == ENDOFFILE;
}

Token Compiler::Previous()
{
    return Tokens[Current - 1];
}

Token Compiler::Advance()
{
    if(!AtEndOfFile()){ Current++; }
    return Previous();
}

/*
  *    Program
   */

Program Compiler::Compile()
{
    Program NewProgram;

    Expr _Expr = ExprExpression();
    // int RegisterIndex = 0;
    // int FirstConstantRegIndex = RegisterIndex;
    // NewProgram.PushInstruction({LOADCONSTANT, FirstConstantRegIndex, NewProgram.PushConstant(1)});

    // RegisterIndex++;

    // int SecondConstantRegIndex = RegisterIndex;

    // NewProgram.PushInstruction({LOADCONSTANT, SecondConstantRegIndex, NewProgram.PushConstant(2)});

    // RegisterIndex++;
    // int AddRegStoreIndex = RegisterIndex;
    
    // NewProgram.PushInstruction({ADD, AddRegStoreIndex,
    //         FirstConstantRegIndex, SecondConstantRegIndex});

    // NewProgram.PushInstruction({PRINT, AddRegStoreIndex});

    // NewProgram.PushInstruction({HALT});

    return NewProgram;
}

void Compiler::Execute()
{
    Program _Program = Compile();
    // CurVM.Load(_Program);
    // CurVM.Run();
}

const Token Compiler::Consume(TokenType TokenA, String Message)
{
    if(TokenA == Peek().Type)
    {
        return Advance();
    }

    Errf("[Error] {}", Message.Value);
}

const Token Compiler::Peek()
{
    return Tokens[Current];
}

bool Compiler::Match(const TokenType TokenA)
{
    if(Peek().Type == TokenA)
    {
        Advance();
        return true;
    }
    
    return false;
}

