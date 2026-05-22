#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <cglm/cglm.h>
#include <stdbool.h>

typedef struct {
    GLchar* vertex_source;
    GLchar* fragment_source;
    const char* path_to_vert;
    const char* path_to_frag;
    GLuint shader_ID;            // linked program
} shader_t;

bool init_shader(shader_t* shader, const char* vert_path, const char* frag_path);
bool reload_shader(shader_t* shader);
void free_shader(shader_t* shader);

void poll_shaders(void);
void use_shader(const shader_t* shader);
void load_error_shader(shader_t* shader, shader_t* err);

void set_bool  (GLuint id, const char* name, bool value);
void set_int   (GLuint id, const char* name, int value);
void set_float (GLuint id, const char* name, float value);
void set_vec3  (GLuint id, const char* name, vec3 value);
void set_matrix(GLuint id, const char* name, mat4 mat);

GLchar* get_shader_source(const char* filename);

#endif
