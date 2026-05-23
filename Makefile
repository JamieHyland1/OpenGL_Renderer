# Compilers
CC  = gcc
CXX = g++

# Output
TARGET = renderer

# Sources and objects
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

# Includes
INCLUDES = -I./include -I./include/headers

# Libraries
LIBS = -lmingw32 -lcimgui -lSDL2main -lSDL2 -lopengl32 -lm -lglew32 -lcglm -lassimp
# Flags
CFLAGS = -Wall -std=c99 $(INCLUDES) -DCIMGUI_USE_SDL2 -DCIMGUI_USE_OPENGL3 -DCIMGUI_DEFINE_ENUMS_AND_STRUCTS

# Compile each .c with gcc, link all .o with g++ (for libstdc++ / cimgui)
build: $(OBJS)
	$(CXX) -o ./$(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug-build: $(OBJS)
	$(CXX) -o ./$(TARGET) -g $(OBJS) $(LIBS)

debug:
	gdb $(TARGET).exe

run:
	./$(TARGET)

clean:
	-del /Q renderer.exe src\*.o 2>nul || rm -f $(TARGET).exe $(OBJS)

.PHONY: build debug-build debug run clean