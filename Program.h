#include "Value.h"

enum class OpCode 
{
    LOADCONSTANT,
    ADD,   // Add 2, 0, 1 (2 = 0 + 1)
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    PRINT, // Print R2
    HALT   // Halt the VM Program
    
};


struct Instruction
{
    OpCode Op;
    std::vector<int> Registers = {};
};


class Program
{

private:
    std::vector<std::unique_ptr<Value>> Constants;
    
public:
    std::vector<Instruction> Instructions;

    void PushInstruction(Instruction CodeInstruction)
    {
        Instructions.push_back(CodeInstruction);
    }

    int PushConstant(std::unique_ptr<Value> ConstantValue)
    {
        Constants.push_back(std::move(ConstantValue));
        return (int)Constants.size() - 1;
    }

    Instruction ReadInstruction(int Index)
    {
        return Instructions[Index];
    }

    Value* ReadConstant(int ConstantIndex)
    {
        return Constants[ConstantIndex].get();
    }

    IntValue* ReadIntConstant(int ConstantIndex)
    {
        return static_cast<IntValue*>(ReadConstant(ConstantIndex));
    }

    void PrintInstruction(const Instruction& Instruct);
};
