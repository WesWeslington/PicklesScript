# link.exe needs a writable temp dir; make's shell leaves TMP empty (LNK1104).
export TMP  := .
export TEMP := .

CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -g
TARGET := main

# Recursive .cpp search (pure-make; avoids shell glob issues on Windows)
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC := $(call rwildcard,.,*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all

# Pass extra args positionally: `make run path/to/file`
run: main
	./main $(RUN_ARGS)

# Treat words after `run` as program args, not targets (phony so no "up to date").
ifeq ($(firstword $(MAKECMDGOALS)),run)
RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
.PHONY: $(RUN_ARGS)
endif

.PHONY: all clean re run
