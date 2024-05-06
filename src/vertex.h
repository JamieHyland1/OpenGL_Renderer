#ifndef vertex_h
#define vertex_h

#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"

typedef struct{
    vec3 position;
    vec3 Normal;
    vec2 TexCoords;
}vertex_t;
#endif