CC=g++
CFLAGS=-Wall
SDL2=-Isrc/include/SDL2

SDL2_FLAG=-Lsrc/lib
SDL2_LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2

ONION=build/onion.exe
ONION_OBJ=build/onion/onion.o build/onion/main.o

PIKMIN=build/pikmin.exe
PIKMIN_OBJ=build/pikmin/pikmin.o build/pikmin/main.o

onion: $(ONION)
	./$(ONION)

pikmin: $(PIKMIN)
	./$(PIKMIN)

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
