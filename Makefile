# Compiler
CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -g

# Target
TARGET := main

# Find all .cpp files recursively
SRC := $(shell find . -name "*.cpp")

# Object files (mirror structure)
OBJ := $(SRC:.cpp=.o)

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild
re: clean all

# Run
run: main
	./main

.PHONY: all clean re
