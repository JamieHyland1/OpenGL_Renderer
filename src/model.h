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
}model_t;

void load_model(model_t* model, char* path);
void process_node(model_t* model, struct aiNode* node, const struct aiScene* scene);
mesh_t process_mesh(struct aiMesh* ai_mesh, const struct aiScene* scene, char* directory);
void load_material_textures(mesh_t* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory);
void draw_model(model_t* model,shader_t* shader);
#endif