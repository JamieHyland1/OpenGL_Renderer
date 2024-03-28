#ifndef shader_h
#define shader_h
#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdbool.h>
#include <C:\SDL2\include\SDL.h>
#include "../include/cglm/cglm.h"
typedef struct{
    const GLchar* vertex_source;
    const GLchar* fragment_source;
    unsigned int shader_ID;
    unsigned int vertex_shader_ID;
    unsigned int fragment_shader_ID;
}shader_t;

bool init_shader(shader_t* shader, char* shader_filenames[2]);
void use_shader(int id);
void set_bool  (int id, char* name, bool value);
void set_int   (int id, char* name, int value);
void set_float (int id, char* name, float value);
void set_matrix(int id, char* name, mat4 mat);
void set_vec3 (int id, char* name, vec3 color); 
GLchar* get_shader_source(char* filename);

#endif