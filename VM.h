#include <vector>

enum OpCode 
{
    LOADCONSTANT,
    ADD,   // Add a, b, c (a = b + c)
    PRINT, // Print a 
    HALT   // Halt the VM Program
    
};

struct Instruction
{
    OpCode Op;
    int R0,R1,R2;
    int Immediate;
};

class Program
{

private:
    std::vector<Instruction> Instructions;
    std::vector<int> Constants;
    
public:
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
