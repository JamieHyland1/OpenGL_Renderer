#ifndef vertex_h
#define vertex_h

#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include <cglm/struct.h>   

typedef struct{
    vec3s Position;
    vec3s Normal;
    vec2s TexCoords;
}vertex_t;
#endif
