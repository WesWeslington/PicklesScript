#include "Scanner.h"
#include "Logging.h"
#include <fstream>
#include <iostream>

String LoadFile(const std::string &Path)
{
    std::ifstream File(Path);
    if (!File)
    {
        ERR("Failed to open file at path {}", Path);
    }

    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );
}

void Run(String Source)
{
    LOG(Source);
}

void RunREPL()
{
    for (;;)
    {
	std::string REPLLine;
        std::cout << ">";
	std::getline(std::cin, REPLLine);
	Run(REPLLine);
    }
}

void RunFile(String SourcePath)
{
    const String SourceText(LoadFile(SourcePath));
    Run(SourceText);
}


int main(int count, char* args[])
{
    LOG(STRING("Hi"));
    if (count == 1)
    {
	RunREPL();
    }
    else if (count == 2)
    {
	RunFile(STRING(args[1]));
    }
    else
    {

    }
    return 0;
}
