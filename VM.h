#include <vector>

enum class OpCode 
{
    LOADCONSTANT,
    ADD,   // Add a, b, c (a = b + c)
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    PRINT, // Print a 
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
    std::vector<int> Constants;
    
public:
    std::vector<Instruction> Instructions;

    void PushInstruction(Instruction CodeInstruction)
    {
        Instructions.push_back(CodeInstruction);
    }

    int PushConstant(int ConstantValue)
    {
        Constants.push_back(ConstantValue);
        return (int)Constants.size() - 1;
    }

    Instruction ReadInstruction(int Index)
    {
        return Instructions[Index];
    }

    int ReadConstant(int ConstantIndex)
    {
        return Constants[ConstantIndex];
    }

    void PrintInstruction(const Instruction& Instruct);
};

class VM
{
private:
    int Registers[3];
    int IP = 0;

    Program* CurProgram = nullptr;
    Instruction ReadInstruction()
    {
        int CurrentIP = IP;
        IP++;
        return CurProgram->ReadInstruction(CurrentIP);
    }
public:
    void Load(Program& NewProgram)
    {
        CurProgram = &NewProgram;
    }

    void Run();
};
