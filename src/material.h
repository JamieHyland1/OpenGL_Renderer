#ifndef material_h
#define material_h

#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material_t;


void set_material_ambient(material_t*   mat,  vec3 ambient);
void set_material_diffuse(material_t*   mat,  vec3 diffuse);
void set_material_specular(material_t*  mat,  vec3 specular);
void set_material_shininess(material_t* mat, float shininess);
#endif