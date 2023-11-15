CC=g++
CFLAGS=-Wall -std=c++17

HEADERS=-Isrc/include/SDL2 -Isrc/include/json -Isrc/include
SDL2_FLAG=-Lsrc/lib
SDL2_LIBS=-lmingw32 -lSDL2main -lSDL2_image -lSDL2 -lSDL2_mixer

SRC_DIR=src/
BUILD_DIR=build/
OBJECT_DIR=$(BUILD_DIR)objects
RELEASE_DIR=releases/

# Build targets.
ONION=$(BUILD_DIR)onion.exe
ONION_OBJ := $(OBJECT_DIR)/main.o \
			 $(OBJECT_DIR)/onion.o \
			 $(OBJECT_DIR)/pikmin.o \
			 $(OBJECT_DIR)/seed.o \
			 $(OBJECT_DIR)/spritesheet.o \
			 $(OBJECT_DIR)/sounds.o \
			 $(OBJECT_DIR)/sprite.o

onion: build
	./$(ONION)

build: $(ONION)

release: RELEASE_NAME := latest
release: build | $(RELEASE_DIR)
	@echo Packaging application for $(RELEASE_NAME) release.
	rm -rf $(RELEASE_DIR)$(RELEASE_NAME)
	mkdir -p $(RELEASE_DIR)$(RELEASE_NAME)
	cp -r assets/ $(RELEASE_DIR)$(RELEASE_NAME)
	cp $(ONION) $(RELEASE_DIR)$(RELEASE_NAME)
	cp $(BUILD_DIR)*.dll $(RELEASE_DIR)$(RELEASE_NAME)
	rm -f $(RELEASE_DIR)$(RELEASE_NAME).tar.gz
	rm -f $(RELEASE_DIR)$(RELEASE_NAME).zip
	tar czvf $(RELEASE_DIR)$(RELEASE_NAME).tar.gz $(RELEASE_DIR)$(RELEASE_NAME)
	zip -r $(RELEASE_DIR)$(RELEASE_NAME).zip $(RELEASE_DIR)$(RELEASE_NAME)

# Link all the generated object files into one executable.
$(ONION): $(ONION_OBJ)
	$(CC) -o $@ $^ $(SDL2_FLAG) $(SDL2_LIBS)

# Compiles the .c files into .o files ready to be linked.
$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR) $(OBJECT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(HEADERS)

# Creates the build directory.
$(BUILD_DIR) $(OBJECT_DIR) $(RELEASE_DIR):
	mkdir $(subst /,\,$@)

kill:
	utils/kill.bat

clean:
	rm -rf build/objects/*.o build/*.exe

.PHONY: all build clean
