#include "../include/GL/glew.h"
#include "C:/SDL2/include/SDL.h"
#include "C:/SDL2/include/SDL_opengl.h"
#include "../include/cglm/cglm.h"
#include "../include/cglm/struct.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/postprocess.h"
#include "../include/assimp/material.h"
#include "stb_image.h"
#include "array.h"
#include "texture.h"
#include "model.h"

void load_model(model_t* model, char* path){
    const struct aiScene* scene = aiImportFile(path, (aiProcess_Triangulate));
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }

    char* directory = strdup(path);
    char* last_slash = strrchr(directory, '/');
    if (last_slash != NULL) {
        last_slash[1] = '\0';
    }
    model->directory = strdup(directory);
    process_node(model, scene->mRootNode, scene);
}


void process_node(model_t* model, struct aiNode* node, const struct aiScene* scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]]; 
        mesh_t mesh;
        mesh = process_mesh(aimesh, scene, model->directory);
        array_push(model->meshes,mesh);
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i ++){
        process_node(model,node->mChildren[i],scene);
    }
}
mesh_t process_mesh(struct aiMesh* aimesh, const struct aiScene* scene, char* directory){
    mesh_t msh = {0};
    int N_VERTS = aimesh->mNumVertices;
    
    for(unsigned int i = 0; i < N_VERTS; i++){
        vertex_t vertex;
        vertex.Position.x = aimesh->mVertices[i].x;
        vertex.Position.y = aimesh->mVertices[i].y;
        vertex.Position.z = aimesh->mVertices[i].z;

        // process vertex positions, normals and texture coordinates

        vertex.Normal.x = aimesh->mNormals[i].x;
        vertex.Normal.y = aimesh->mNormals[i].y;
        vertex.Normal.z = aimesh->mNormals[i].z;

        if(aimesh->mTextureCoords[0]){
            vertex.TexCoords.x = aimesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = aimesh->mTextureCoords[0][i].y;
        }
        else{
            vertex.TexCoords.x = 0.0f;
            vertex.TexCoords.y = 0.0f; 
        }
        
        
        array_push(msh.vertices,vertex);
    }
    // process indices
    for(unsigned int i = 0; i < aimesh->mNumFaces; i ++){
        struct aiFace face =  aimesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            array_push(msh.indices, face.mIndices[j]);
        }
    }
    // process material
    if(aimesh->mMaterialIndex >= 0){
        struct aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        load_material_textures(&msh, material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
        load_material_textures(&msh, material, aiTextureType_SPECULAR,"texture_specular",directory);
    }

    return msh;
}
void load_material_textures(mesh_t* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory){
    int tex_count = aiGetMaterialTextureCount(mat,type);
    for(unsigned int i = 0; i < tex_count; i++){
        struct aiString str;
        enum aiReturn rtrn = aiGetMaterialTexture(mat,type,i,&str,NULL,NULL,NULL,NULL,NULL,NULL);
        if(rtrn == aiReturn_SUCCESS){
            texture_t texture = init_texture(str.data);
            texture.type = typeName;
            array_push(mesh->textures,texture);
        }
        else if(rtrn == aiReturn_FAILURE){
            printf("error loading texture data\n");
        }
    }
}

void draw_model(model_t* model,shader_t* shader){
    int numMeshes = array_length(model->meshes);
    for(unsigned int i = 0; i < numMeshes; i++){
        draw_mesh(&model->meshes[i],shader);
    }
}