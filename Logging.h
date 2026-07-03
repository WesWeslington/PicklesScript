#include <stdexcept>
#include <iostream>

inline void Log(const std::string& Message)
{
    std::cout << Message << '\n';
}

inline std::runtime_error Err(std::string Message)
{
    throw std::runtime_error(Message.data());
}

#define Logf(Message, ...)\
{\
    std::cout << std::format(Message, __VA_ARGS__) << '\n';\
}

#define Errf(Message, ...)\
{\
    throw Err(std::format(Message, __VA_ARGS__));       \
}
