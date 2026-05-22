#ifndef vertex_h
#define vertex_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/struct.h>   

typedef struct{
    vec3s Position;
    vec3s Normal;
    vec2s TexCoords;
}vertex_t;
#endif
