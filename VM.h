#include <vector>
#include "Program.h"


class VM
{
private:
    // TODO: Split reg
    std::vector<int> Registers = std::vector<int>(256);
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

    int GetValFromRegister(const int& Index) { return Registers[Index]; }

    IntValue* ReadIntFromRegister(int InstructionRegister, const Instruction& CurInstruction);

};
