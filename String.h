#include <format>
#include <string>


class String
{
 public:
    std::string Value = "";

    String(const std::string& NewString):Value(NewString){}
    String():Value(""){}
    String(const String& rhs):Value(rhs.Value){}
    String& operator=(const String& rhs);

    operator std::string() const
    {
        return Value;
    }

    operator const char*() const
    {
        return Value.data();
    }
};
