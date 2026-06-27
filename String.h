#include <format>
#include <string>


#define STRING(message) String(message)
#define STRINGF(message, ...) String(std::format(message, __VA_ARGS__))

class String
{
 public:
    String(std::string Value):Value(Value){}
    
    std::string Value;

    operator std::string() const
    {
        return Value;
    }

    operator const char*() const
    {
        return Value.data();
    }
};
