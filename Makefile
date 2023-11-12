CC=g++
CFLAGS=-Wall

HEADERS=-Isrc/include/SDL2 -Isrc/include/json
SDL2_FLAG=-Lsrc/lib
SDL2_LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2

# Build targets.
ONION=build/onion.exe
PIKMIN=build/pikmin.exe

SRC_DIRS=src/onion src/pikmin src/spritesheet
BUILD_DIRS=build\onion build\pikmin build\spritesheet

ONION_OBJ := build/onion/onion.o build/onion/main.o build/spritesheet/spritesheet.o
# ONION_OBJ=build/onion/onion.o build/onion/main.o build/spritesheet/spritesheet.cpp
# PIKMIN_OBJ=build/pikmin/pikmin.o build/pikmin/main.o

all: $(ONION)

onion: $(ONION)
	./$(ONION)

pikmin: $(PIKMIN)
	./$(PIKMIN)

$(ONION): $(ONION_OBJ)
	$(CC) -o $@ $^ $(SDL2_FLAG) $(SDL2_LIBS)

$(PIKMIN): $(PIKMIN_OBJ)
	$(CC) -o $@ $^ $(SDL2_FLAG) $(SDL2_LIBS)
# $(CC) -o $@ $(PIKMIN_OBJ) $(SDL2_FLAG) $(SDL2_LIBS)

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
