# Compiler
CC = gcc

# Output executable
TARGET = renderer

# Source files
SRCS = ./src/*.c

# Include paths
INCLUDES = -IC:/SDL2/include \
           -IC:/Assimp/include \
           -IC:/Assimp/build/include \
           -IC:/cglm/include \
           -IC:/glew-2.1.0/include

# Library paths
LIB_PATHS = -LC:/SDL2/lib \
            -LC:/glew-2.1.0/lib/Release/x64 \
            -LC:/cglm/build \
            -LC:/Assimp/build/lib \
            -LC:/Assimp/build/bin

# Libraries (order matters: mingw32 must come before SDL2main)
LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lglew32 -lcglm -lassimp

# Compiler flags
CFLAGS = -Wall -std=c99
DEBUG_FLAGS = -g

# Default target
build:
	cls
	$(CC) -o ./$(TARGET) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIB_PATHS) $(LIBS)

# Debug build
debug-build:
	$(CC) -o ./$(TARGET) $(DEBUG_FLAGS) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIB_PATHS) $(LIBS)

# Debug with GDB
debug:
	gdb $(TARGET).exe

# Run the program
run:
	./$(TARGET)

# Clean build artifacts
clean:
	rm $(TARGET).exe

.PHONY: build debug-build debug run clean