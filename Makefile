CC := g++
BIN := not_dual

SRC := src
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, build/obj/%.o, $(SRCS))
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lpthread

all: $(OBJS)
	$(CC) -o $(BIN) $^ $(LIBS)

build/obj/%.o: src/%.cpp | build/obj
	$(CC) -c $< -o $@

build/obj:
	mkdir -p build/obj

clean:
	rm -rf build
	rm -f $(BIN)
