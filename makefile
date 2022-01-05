CC = g++
INCLUDES = -I. -I./src
LDFLAGS = 
CFLAGS = -Wall -std=gnu++17 $(INCLUDES)

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
DEPS = $(wildcard src/*.hpp)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $<

main.e: $(OBJ)
	$(CC) $^ $(LDFLAGS) -o bin/$@

crun: clean main.e
	./bin/main.e

compile: main.e

run:
	./bin/main.e

.PHONY: clean
clean:
	rm -f src/*.o bin/main.e