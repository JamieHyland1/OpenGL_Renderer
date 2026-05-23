#ifndef skybox_h
#define skybox_h

#include <GL/glew.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include "shader.h"

typedef struct {
    GLuint texture_id;
} cubemap_t;                    

typedef struct {
    cubemap_t cubemap;
    GLuint VAO;
    GLuint VBO;
    shader_t shader;
} skybox_t;

extern skybox_t skybox;

cubemap_t load_cubemap(const char* faces[6]);
bool init_skybox(skybox_t* sky, const char* faces[6],
                 const char* vert_path, const char* frag_path);
void draw_skybox(const skybox_t* sky, mat4 view, mat4 projection);
void free_skybox(skybox_t* sky);

#endif
