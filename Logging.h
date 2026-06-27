#include <string>
#include <stdexcept>
#include <iostream>

#define LOG(message) std::cout << message << '\n'

#define ERR(message) throw std::runtime_error(message)
