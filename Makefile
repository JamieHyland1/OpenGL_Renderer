build:
	gcc -o renderer  -Wall -std=c99 ./src/*.c -IC:/SDL2/include -lSDL2main  -lopengl32 -lm -lSDL2 -LC:/SDL2/lib/x64 -LC:\glew-2.1.0\lib\Release\x64 -lglew32
	./renderer
debug-build:
	gcc -o renderer -g  -Wall -std=c99 ./src/*.c -IC:/SDL2/include -lSDL2main -lopengl32 -lm -lSDL2 -LC:/SDL2/lib/x64
debug:
	gdb  renderer.exe 
run:
	./renderer
clean:
	del renderer.exe