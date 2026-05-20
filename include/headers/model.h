#ifndef model_h
#define model_h

#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include "../include/cglm/struct.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/postprocess.h"
#include "../include/assimp/material.h"

#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"

typedef struct{
    mesh_t* meshes;
    char* directory;
    size_t num_meshes;
}model_t;

bool load_model(model_t* model, char* path);
void draw_model(model_t* model,shader_t* shader);
void free_model(model_t* model);
#endif
