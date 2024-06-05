#ifndef mesh_h
#define mesh_h
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include "../include/cglm/struct.h"
#include "vertex.h"
#include "texture.h"
#include "shader.h"
// /////////////////////////////////////////////////////////////////////////////////////////
// // Define a struct for dynamic size meshes
// ////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    vertex_t* vertices; // Dynamic array
    texture_t* textures; // Mesh png texture pointer
    int* indices; // dynamic array
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} mesh_t;
void setup_mesh(mesh_t* mesh);
void draw_mesh(mesh_t* mesh, shader_t* shader);
#endif