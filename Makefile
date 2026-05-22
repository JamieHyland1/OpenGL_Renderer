# Compiler
CC = gcc

# Output
TARGET = renderer

# Sources
SRCS = $(wildcard src/*.c)

# Only your project headers - all libraries are in MSYS2 system paths
INCLUDES = -I./include -I./include/headers

# Libraries
LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lglew32 -lcglm -lassimp

# Flags
CFLAGS = -Wall -std=c99

build:
	$(CC) -o ./$(TARGET) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIBS)

debug-build:
	$(CC) -o ./$(TARGET) -g $(CFLAGS) $(SRCS) $(INCLUDES) $(LIBS)

debug:
	gdb $(TARGET).exe

run:
	./$(TARGET)

clean:
	rm -f $(TARGET).exe

.PHONY: build debug-build debug run clean