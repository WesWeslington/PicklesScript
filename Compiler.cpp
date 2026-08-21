#include "Compiler.h"
#include "Logging.h"

VM CurVM;

/*
  *    Expression
   */
Expr Compiler::ExprExpression()
{
    return ExprEquality();
}

Expr Compiler::ExprEquality()
{
    Expr _Expr = ExprComparison();
    while(Match(TokenType::BANG_EQUAL) || Match(TokenType::EQUAL_EQUAL))
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

    while(Match(TokenType::GREATER) || Match(TokenType::GREATER_EQUAL) ||
          Match(TokenType::LESS) || Match(TokenType::LESS_EQUAL) )
    {
        Token Operator = Previous();
        Expr Right = ExprTerm();

        switch(Operator.Type)
        {
        case TokenType::GREATER:
            break;
        case TokenType::LESS:
            break;
        case TokenType::GREATER_EQUAL:
            break;
        case TokenType::LESS_EQUAL:
            break;
        default:
            Err("Expected GREATER / LESS / GREATER_EQUAL / LESS_EQUAL in ExprComparison");
            break;
        }
        return Binary{Right, Operator, _Expr};
    }
    
    return _Expr;      
}

Expr Compiler::ExprTerm()
{
    Expr _Expr = ExprFactor();

    while(Match(TokenType::PLUS) || Match(TokenType::MINUS))
    {
        Token Operator = Previous();
        Expr Right = ExprFactor();
        switch(Operator.Type)
        {
        case TokenType::PLUS:
            _Program.PushInstruction({OpCode::ADD, {2, 0, 1}});
            break;
        case TokenType::MINUS:
            _Program.PushInstruction({OpCode::SUBTRACT, {2, 0, 1}});
            break;
        default:
            Log("Default operator type hit. nothin gona happen cuh");
            break;
        }

        return Binary{Right, Operator, _Expr};
    }

    return _Expr;
}

Expr Compiler::ExprFactor()
{
    Expr _Expr = ExprUnary();

    while(Match(TokenType::STAR) || Match(TokenType::SLASH))
    {
        Token Operator = Previous();
        Expr Right = ExprUnary();

        switch(Operator.Type)
        {
        case TokenType::STAR:
            _Program.PushInstruction({OpCode::MULTIPLY, {2, 0, 1}});
            break;
        case TokenType::SLASH:
            _Program.PushInstruction({OpCode::DIVIDE, {2, 0, 1}});
            break;
        default:
            Log("Expected STAR / SLASH in ExprFactor");
            break;
        }

        return Binary{Right, Operator, _Expr};
    }

    return _Expr;
}

Expr Compiler::ExprUnary()
{
    if(Match(TokenType::MINUS) || Match(TokenType::BANG))
    {
        Token Operator = Previous();
        Expr Right = ExprLiteral();
        return Unary{Right, Operator};
    }

    return ExprLiteral();
}

Expr Compiler::ExprLiteral()
{
    if(Match(TokenType::FALSE))
    {
        int PushedConstantIndex = _Program.PushConstant(std::make_unique<BoolValue>(BoolValue{false}));
        _Program.PushInstruction({
                OpCode::LOADCONSTANT,
                {PushedConstantIndex
                }
            });
        return Literal{false};
    }
    if(Match(TokenType::TRUE))
    {
        int PushedConstantIndex = _Program.PushConstant(std::make_unique<BoolValue>(BoolValue{true}));
        _Program.PushInstruction({
                OpCode::LOADCONSTANT,
                {PushedConstantIndex
                }
            });
        return Literal{true};
    }
    if(Match(TokenType::NAWW)) { Log("Naww");  return Literal{NULL};  }

    if(Match(TokenType::NUMBER))
    {
        IntValue Num = IntValue{std::stoi(Previous().Lexeme)};
        int PushedConstantIndex = _Program.PushConstant(std::make_unique<IntValue>(Num));
        _Program.PushInstruction({
                OpCode::LOADCONSTANT,
                {PushedConstantIndex
                }
            });
        return Literal{Num};
    }

    if(Match(TokenType::STRING))
    {
        StringValue LitString = StringValue{Previous().Lexeme};
        _Program.PushInstruction({OpCode::LOADCONSTANT, {
                    _Program.PushConstant(
                         std::make_unique<StringValue>(LitString))}
            });
        return Literal{LitString};
    }

    if(Match(TokenType::LEFT_PAREN))
    {
        Log("Before");
        Expr _Expr = ExprExpression();
        Log("After");
        Consume(TokenType::RIGHT_PAREN, String("Expected ')' at the end of a grouping"));
        return Grouping{_Expr};
    }

    if(Match(TokenType::ENDOFFILE))
    {
        return Literal{NULL};
    }

    Errf("Expected expression in literal, error at token type {}", Peek().Lexeme);
}

bool Compiler::AtEndOfFile()
{
    return Peek().Type == TokenType::ENDOFFILE;
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
void PrintInstruction(const Instruction& Instruct)
{
    std::string OpcodeString;
    switch(Instruct.Op)
    {
    case OpCode::ADD:
        OpcodeString = "OP_ADD";
        break;
    case OpCode::LOADCONSTANT:
        OpcodeString = "OP_LOADCONSTANT";
        break;
    case OpCode::PRINT:
        OpcodeString = "OP_PRINT";
        break;
    case OpCode::HALT:
        OpcodeString = "OP_HALT";
        break;
    default:
        OpcodeString = "OP_UNSET";
        break;
    }
    
    Logf("Op: {}", OpcodeString);

    // TODO: Print values instead of register locations
    for(size_t Index = 0; Index < Instruct.Registers.size(); Index++)
    {
        Logf("R{}: {}", Index, Instruct.Registers[Index]);
    }
}

void Compiler::DumpBytecode()
{
    for(const auto& Instruct : _Program.Instructions)
    {
        PrintInstruction(Instruct);
    }
}

CompileResult Compiler::Compile()
{
    _Program = Program{};
    while(!Match(TokenType::ENDOFFILE))
    {
        EvaluateExpressions();
    }
    _Program.PushInstruction({OpCode::HALT});
    Log("Expression complete");
    DumpBytecode();
    Execute();
    
    return SUCCESS;
}

void Compiler::Execute()
{
    CurVM.Load(_Program);
    Log("Program loaded");
    CurVM.Run();
    Log("Program ran");
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

/*
  *    Statements
   */

void Compiler::EvaluateExpressions()
{
    if(Match(TokenType::PRINT)){ PrintStatement(); return; }

    ExpressionStatement();
}

void Compiler::PrintStatement()
{
    Expr Expression(ExprExpression());
    Consume(TokenType::SEMICOLON, String("Expect semicolon after 'print'"));
    _Program.PushInstruction({OpCode::PRINT, {2}});
}

void Compiler::ExpressionStatement()
{
    Expr Expression = ExprExpression();
    Consume(TokenType::SEMICOLON, String("Expect semicolon after expression."));
};
