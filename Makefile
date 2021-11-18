BIN := not_dual

CC := g++
CFLAGS := -std=c++20 -Wall -Werror -Wpedantic -g
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lm

SRC_DIR := src
BUILD_DIR := build
INCL_DIR := includes
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
INCLS := $(wildcard $(INCL_DIR)/*.h)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Colors
ESC := \033
RESET := $(ESC)[0m
BRED := $(ESC)[1;31m
BGREEN := $(ESC)[1;32m
BYELLOW := $(ESC)[1;33m
BBLUE := $(ESC)[1;34m

all: $(OBJS) | $(BIN_DIR)/
	@printf "\t$(BGREEN)LINK$(RESET) \t$^ $(LIBS) -> $(BIN)\n"
	@$(CC) $(CFLAGS) -o $(BIN) $^ $(LIBS)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)/
	@printf "\t$(BBLUE)CC$(RESET)   \t$< -> $@\n"
	@$(CC) $(CFLAGS) -c $< -o $@ -I.

# Tell make that this is not an intermediate file.
.PRECIOUS: %/
%/:
	@printf "\t$(BYELLOW)MKDIR$(RESET)\t$(patsubst %/,%,$@)\n"
	@mkdir -p $@

.PHONY: clean
clean:
	@printf "\t$(BRED)RM$(RESET)   \t$(BUILD_DIR)\n"
	@rm -rf build
	@printf "\t$(BRED)RM$(RESET)   \t$(BIN)\n"
	@rm -f $(BIN)
