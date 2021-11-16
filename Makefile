CC := g++
BIN := not_dual

SRC := src
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, build/obj/%.o, $(SRCS))
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

all: $(OBJS)
	$(CC) -o $(BIN) $< $(LIBS)

build/obj/%.o: src/%.c | build/obj
	$(CC) -c $< -o $@

build/obj:
	mkdir -p build/obj

clean:
	rm -rf build
	rm -f $(BIN)
