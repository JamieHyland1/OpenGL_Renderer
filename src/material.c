#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "material.h"

void set_material_ambient(material_t* mat,  vec3 ambient){
    glm_vec3_copy(&ambient[0],&mat->ambient[0]);
}
void set_material_diffuse(material_t* mat,  vec3 diffuse){
    glm_vec3_copy(&diffuse[0],&mat->diffuse[0]);
}
void set_material_specular(material_t* mat, vec3 specular){
    glm_vec3_copy(&specular[0],&mat->specular[0]);
}
void set_material_shininess(material_t* mat, float shininess){
    mat->shininess = shininess;
}