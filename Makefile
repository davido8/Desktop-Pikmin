CC=g++
CFLAGS=-Wall
SDL2=-Isrc/include/SDL2
DEPS=src/process.h

SDL2_FLAG=-Lsrc/lib
SDL2_LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2

EXECUTABLE=build/virus.exe
OBJECTS=build/main.o build/process.o

ONION=build/onion.exe
ONION_OBJ=build/onion.o build/main.o# build/debug.o build/process.o

PIKMIN=build/pikmin.exe
PIKMIN_OBJ=build/pikmin.o build/debug.o

run: $(EXECUTABLE) $(ONION)
	./$(EXECUTABLE)

onion: $(ONION)
	./$(ONION)

pikmin: $(PIKMIN)
	./$(PIKMIN)

# Links the compiled .o files into the final executable, along with SDL2.
$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(SDL2_FLAG) $(SDL2_LIBS)
    # $(CC): Specify what compiler to use.
    # -o $@: Specify final name of executable.
    # $<: Specify which files are to be linked together.
    # $(SDL2_FLAG): Specify where to find the SDL2 libraries.
    # $(SDL2_LIBS): Specify what SDL2 libraries we need.

$(ONION): $(ONION_OBJ)
	$(CC) -o $@ $(ONION_OBJ) $(SDL2_FLAG) $(SDL2_LIBS)

$(PIKMIN): $(PIKMIN_OBJ)
	$(CC) -o $@ $(PIKMIN_OBJ) $(SDL2_FLAG) $(SDL2_LIBS)

# Compiles the .c files into .o files ready to be linked.
build/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(SDL2) -Isrc
#	-c: compile into object file.
#	-o $@: output into FILE.o where FILE is the name of the file being compiled. 
#	$<: expands to right side of : (say which file to compile.)
#	$(CFLAGS): provide all flags we need.

clean:
	erase build\*.o build\*.exe
