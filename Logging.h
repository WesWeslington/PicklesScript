#include "String.h"
#include <stdexcept>
#include <iostream>

#define LOG(message) std::cout << message << '\n'

#define ERR(...) throw std::runtime_error(STRINGF(__VA_ARGS__).Value)
