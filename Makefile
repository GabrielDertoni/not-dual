BIN := not_dual

CC := g++
CFLAGS := -std=c++2a -Wall -Werror -Wpedantic
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


# Colors
ESC := \033
RESET := $(ESC)[0m
BRED := $(ESC)[1;31m
BGREEN := $(ESC)[1;32m
BYELLOW := $(ESC)[1;33m
BBLUE := $(ESC)[1;34m

all: $(OBJS) | $(BIN_DIR)/
	@printf "\t$(BGREEN)LINK$(RESET) \t$^ $(LIBS) %%s -> $(BIN)\n"
	@$(CC) $(CFLAGS) -o $(BIN) $^ $(LIBS)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)/
	@printf "\t$(BBLUE)CC$(RESET)   \t$< -> $@\n"
	@$(CC) $(CFLAGS) -c $< -o $@

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
