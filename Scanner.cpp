#include "Scanner.h"
#include <fstream>
#include "Logging.h"
#include "String.h"

String LoadFile(const std::string& Path) {
    std::ifstream File(Path);
    if (!File) {
        ERR(STRINGF("Failed to open file at path {}", Path));
    }

    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );
}

std::vector<Token> ScanTokens(std::string Source)
{
    const String SourceText(LoadFile(Source));
    return {};
}
