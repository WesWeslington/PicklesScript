#include "VM.h"

#include "Logging.h"
#include <iostream>

void PrintValue(Value* Val)
{
    switch(Val->Type())
    {
    case ValueType::Int:
        Log(static_cast<IntValue*>(Val)->AsString());
        break;
    case ValueType::String:
        Log(static_cast<StringValue*>(Val)->AsString());
        break;
    case ValueType::Bool:
        Log(static_cast<BoolValue*>(Val)->AsString());
        break;
    default:
        Log("PrintValue Unreachable Reached");
        break;
    }
}

// Not ideal to store in the constant pool
// In the future when we have code blocks, each block should have its own "pool"
int PushBinaryResult(Program* _Program, int MathResult)
{
    std::unique_ptr<Value> Val = std::make_unique<IntValue>(MathResult);
    return _Program->PushConstant(std::move(Val));
}

IntValue* VM::ReadIntFromRegister(int InstructionRegister, const Instruction& CurInstruction)
{
    return CurProgram->ReadIntConstant(
        GetValFromRegister(CurInstruction.Registers[InstructionRegister]));
}

void VM::Run()
{
    for(;;)
    {
        Instruction CurInstruction = ReadInstruction();
        switch(CurInstruction.Op)
        {
        case OpCode::LOADCONSTANT:
        {

            int ConstantIndex = CurInstruction.Registers[0];
            
            Registers[CurInstruction.Registers[0]] = ConstantIndex;
            break;
        }
        case OpCode::ADD:
        {
            //TODO: Reduce the code between Add / Sub / Multiply / Divide
            // A = B + C
            // R0 = R1 + R2
            int Val1 = *ReadIntFromRegister(1, CurInstruction);
            int Val2 = *ReadIntFromRegister(2, CurInstruction);
            
            int Sum = Val1 + Val2;
            Registers[CurInstruction.Registers[0]] = PushBinaryResult(CurProgram, Sum);
            break;
        }
        case OpCode::SUBTRACT:
        {
            int Val1 =  *ReadIntFromRegister(1, CurInstruction);
            int Val2 =  *ReadIntFromRegister(2, CurInstruction);
            
            int Difference = Val1 - Val2;
            Registers[CurInstruction.Registers[0]] = PushBinaryResult(CurProgram, Difference);
            break;
        }
        case OpCode::DIVIDE:
        {
            int Val1 =  *ReadIntFromRegister(1, CurInstruction);
            int Val2 =  *ReadIntFromRegister(2, CurInstruction);

            int Quotient = Val1 / Val2;
            Registers[CurInstruction.Registers[0]] = PushBinaryResult(CurProgram, Quotient);
            break;
        }
        case OpCode::MULTIPLY:
        {
            int Val1 =  *ReadIntFromRegister(1, CurInstruction);
            int Val2 =  *ReadIntFromRegister(2, CurInstruction);
            
            int Product = Val1 * Val2;
            Registers[CurInstruction.Registers[0]] = PushBinaryResult(CurProgram, Product);
            break;
        }
        case OpCode::PRINT:
        {
            int Index = Registers[CurInstruction.Registers[0]];
            Value* Val = CurProgram->ReadConstant(Index);
            PrintValue(Val);
            break;
        }
        case OpCode::HALT:
        {
            Log("Halt called");
            return;
        }
        default:
            break;
        }
    }
};
