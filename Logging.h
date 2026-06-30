#include "String.h"
#include <stdexcept>
#include <iostream>

inline void Log(const std::string& Message)
{
    std::cout << Message << '\n';
}

inline void Err(std::string Message)
{
    throw std::runtime_error(Message.data());
}

#define Logf(Message, ...)\
{\
    std::cout << std::format(Message, __VA_ARGS__) << '\n';\
}

#define Errf(Message, ...)\
{\
    throw std::runtime_error(std::format(Message, __VA_ARGS__));     \
}
