#ifndef mesh_h
#define mesh_h
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cglm/cglm.h>          
#include <cglm/struct.h>        
#include "texture.h"
#include "shader.h"

// /////////////////////////////////////////////////////////////////////////////////////////
// // Define a struct for dynamic size meshes
// ////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    vertex_t* vertices; 
    texture_t* textures; 
    size_t num_vertices;
    size_t num_textures;
    int* indices; 
    size_t num_indices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} mesh_t;


void setup_mesh(mesh_t* mesh);
void draw_mesh(mesh_t* mesh, shader_t* shader);
#endif
