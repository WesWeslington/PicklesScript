#include "VM.h"

#include <iostream>

void VM::Run()
{
    for(;;)
    {
        Instruction CurInstruction = ReadInstruction();
        switch(CurInstruction.Op)
        {
        case LOADCONSTANT:
        {
            // Constant is in R1
            int ConstantIndex = CurInstruction.R1;
            int Constant = CurProgram->ReadConstant(ConstantIndex);

            // Store constant on R0
            Registers[CurInstruction.R0] = Constant;
            break;
        }
        case ADD:
        {
            // A = B + C
            int Val1 = Registers[CurInstruction.R1];
            int Val2 = Registers[CurInstruction.R2];
            
            int Sum = Val1 + Val2;
            Registers[CurInstruction.R0] = Sum;
            break;
        }
        case PRINT:
        {
            std::cout << Registers[CurInstruction.R0] << std::endl;
            break;
        }
        case HALT:
        {
            return;
        }
        default:
            break;
        }
    }
};
