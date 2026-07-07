#include "Compiler.h"
#include "Logging.h"

#include <fstream>
#include <iostream>

String LoadFile(const std::string &Path)
{
    std::ifstream File(Path);
    if (!File)
    {
        Errf("Failed to open file at path {}", Path);
    }

    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );
}

void Run(String Source)
{
    Scanner CurScanner;
    std::vector<Token> Tokens = CurScanner.ScanTokens(Source.Value + '\0');
    Compiler CurCompiler(Tokens);
    CurCompiler.Compile();
}

void RunREPL()
{
    for (;;)
    {
	String REPLLine;
        std::cout << ">";
	std::getline(std::cin, REPLLine.Value);
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
    Log(String("Hi"));
   
    if (count == 1)
    {
	RunREPL();
    }
    else if (count == 2)
    {
	RunFile(String(args[1]));
    }
    else
    {

    }

    
    
    return 0;
}
