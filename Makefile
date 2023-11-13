CC=g++
CFLAGS=-Wall

HEADERS=-Isrc/include/SDL2 -Isrc/include/json -Isrc/include
SDL2_FLAG=-Lsrc/lib
SDL2_LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -lSDL2_mixer

# Build targets.
ONION=build/onion.exe

SRC_DIRS=src/onion src/pikmin src/spritesheet
BUILD_DIRS=build

ONION_OBJ := build/main.o build/onion.o build/pikmin.o build/seed.o build/spritesheet.o build/sounds.o

all: $(ONION)

onion: $(ONION)
	./$(ONION)

$(ONION): $(ONION_OBJ)
	$(CC) -o $@ $^ $(SDL2_FLAG) $(SDL2_LIBS)

# Compiles the .c files into .o files ready to be linked.
build/%.o: src/%.cpp | $(BUILD_DIRS)
	$(CC) -c -o $@ $< $(CFLAGS) $(HEADERS) -Isrc
#	-c: compile into object file.
#	-o $@: output into FILE.o where FILE is the name of the file being compiled. 
#	$<: expands to right side of : (say which file to compile.)
#	$(CFLAGS): provide all flags we need.

$(BUILD_DIRS):
	mkdir $@

clean:
	erase build\*.o build\*.exe

.PHONY: all clean
