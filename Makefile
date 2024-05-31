build:
	gcc -o ./renderer -Wall -std=c99 ./src/*.c -IC:/SDL2/include -IC:/Assimp/include -LC:/SDL2/lib/x64 -lSDL2main -lSDL2 -lopengl32 -lm -LC:/glew-2.1.0/lib/Release/x64 -lglew32 -LC:/cglm-master/build -lcglm-0 -LC:/Assimp/bin -lassimp-vc142-mtd
	./renderer
debug-build:
	gcc -o ./renderer -g -Wall -std=c99 ./src/*.c -IC:/SDL2/include -IC:/Assimp/include -LC:/SDL2/lib/x64 -lSDL2main -lSDL2 -lopengl32 -lm -LC:/glew-2.1.0/lib/Release/x64 -lglew32 -LC:/cglm-master/build -lcglm-0 -LC:/Assimp/bin -lassimp-vc142-mtd
debug:
	gdb  renderer.exe 
run:
	./renderer
clean:
	del renderer.exe