#include "VM.h"

#include "Logging.h"
#include <iostream>

void VM::Run()
{
    for(;;)
    {
        Instruction CurInstruction = ReadInstruction();
        switch(CurInstruction.Op)
        {
        case OpCode::LOADCONSTANT:
        {
            // Constant is in R1
            int ConstantIndex = CurInstruction.Registers[1];
            int Constant = CurProgram->ReadConstant(ConstantIndex);

            Log("Load constant called");
            // Store constant on R0
            Registers[CurInstruction.Registers[0]] = Constant;
            break;
        }
        case OpCode::ADD:
        {
            //TODO: Reduce the code between Add / Sub / Multiply / Divide
            // A = B + C
            int Val1 = Registers[CurInstruction.Registers[1]];
            int Val2 = Registers[CurInstruction.Registers[2]];
            
            int Sum = Val1 + Val2;
            Registers[CurInstruction.Registers[0]] = Sum;
            break;
        }
        case OpCode::SUBTRACT:
        {
            int Val1 = Registers[CurInstruction.Registers[1]];
            int Val2 = Registers[CurInstruction.Registers[2]];
            
            int Difference = Val1 - Val2;
            Registers[CurInstruction.Registers[0]] = Difference;
            break;
        }
        case OpCode::DIVIDE:
        {
            int Val1 = Registers[CurInstruction.Registers[1]];
            int Val2 = Registers[CurInstruction.Registers[2]];
            
            int Quotient = Val1 / Val2;
            Registers[CurInstruction.Registers[0]] = Quotient;
            break;
        }
        case OpCode::MULTIPLY:
        {
            int Val1 = Registers[CurInstruction.Registers[1]];
            int Val2 = Registers[CurInstruction.Registers[2]];
            
            int Product = Val1 * Val2;
            Registers[CurInstruction.Registers[0]] = Product;
            break;
        }
        case OpCode::PRINT:
        {
            std::cout << Registers[CurInstruction.Registers[0]] << std::endl;
            Log("Print  called");
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
