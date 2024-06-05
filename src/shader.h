#ifndef shader_h
#define shader_h
#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdbool.h>
#include <C:\SDL2\include\SDL.h>
#include "../include/cglm/cglm.h"
#include "fileapi.h"
#include "synchapi.h"
typedef struct{
    const GLchar* vertex_source;
    const GLchar* fragment_source;
    const GLchar* path_to_frag;
    const GLchar* path_to_vert;
    GLuint shader_ID;
    unsigned int vertex_shader_ID;
    unsigned int fragment_shader_ID;
}shader_t;

typedef enum {VERTEX,FRAGMENT}Shader_Type;
bool init_shader(shader_t* shader, char* filename, Shader_Type type);
bool link_shader(shader_t* shader);
bool reload_shader(shader_t* shader);
void poll_shaders();
void CALLBACK ShaderChangedCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
void use_shader(int id);
void load_error_shader(shader_t* shader, shader_t* err);
void set_bool  (int id, char* name, bool value);
void set_int   (int id, char* name, int value);
void set_float (int id, char* name, float value);
void set_matrix(int id, char* name, mat4 mat);
void set_vec3 (int id, char* name, vec3 color); 
GLchar* get_shader_source(char* filename);

#endif