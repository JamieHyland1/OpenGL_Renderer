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
    const struct aiScene* scene = aiImportFile(path, (aiProcess_Triangulate | aiProcess_FlipUVs));
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }

    char* directory = strdup(path);
    char* last_slash = strrchr(directory, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
    model->directory = strdup(path);
    process_node(model, scene->mRootNode, scene);
}


void process_node(model_t* model, struct aiNode* node, const struct aiScene* scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]]; 
        mesh_t mesh;
        mesh = process_mesh(aimesh, scene);
        array_push(model->meshes,mesh);
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i ++){
        process_node(model,node->mChildren[i],scene);
    }
}
mesh_t process_mesh(struct aiMesh* aimesh, const struct aiScene* scene){
    mesh_t msh = {0};
    int N_VERTS = aimesh->mNumVertices;
    //printf("Number of numVertices: [%d]\n",N_VERTS);
    //printf("mesh vertices array length: [%d]\n", array_length(mesh->vertices));
    for(unsigned int i = 0; i < N_VERTS; i++){
        vertex_t vertex;
        vertex.Position.x = aimesh->mVertices[i].x;
        vertex.Position.y = aimesh->mVertices[i].y;
        vertex.Position.z = aimesh->mVertices[i].z;

        // printf("vertex[%d].Position.x: [%f]\n",i, vertex.Position.x);
        // printf("vertex[%d].Position.y: [%f]\n",i, vertex.Position.y);
        // printf("vertex[%d].Position.z: [%f]\n",i, vertex.Position.z);
        // process vertex positions, normals and texture coordinates

        vertex.Normal.x = aimesh->mNormals[i].x;
        vertex.Normal.y = aimesh->mNormals[i].y;
        vertex.Normal.z = aimesh->mNormals[i].z;

        // printf("vertex[%d].Normal.x: [%f]\n",i, vertex.Normal.x);
        // printf("vertex[%d].Normal.y: [%f]\n",i, vertex.Normal.y);
        // printf("vertex[%d].Normal.z: [%f]\n",i, vertex.Normal.z);

        if(aimesh->mTextureCoords[0]){
    
            vertex.TexCoords.x = aimesh->mTextureCoords[0]->x;
            vertex.TexCoords.y = aimesh->mTextureCoords[0]->y;
    
        }
        else{
            vertex.TexCoords.x = 0.0f;
            vertex.TexCoords.y = 0.0f; 
        }
        // printf("vertex[%d].TexCoords.x: [%f]\n",i, vertex.TexCoords.x);
        // printf("vertex[%d].TexCoords.y: [%f]\n",i, vertex.TexCoords.y);
        
        array_push(msh.vertices,vertex);
       // printf("mesh vertices array length: [%d]\n", array_length(msh.vertices));
        
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
        load_material_textures(msh.textures, material, aiTextureType_DIFFUSE, "texture_diffuse");
        load_material_textures(msh.textures, material, aiTextureType_SPECULAR,"texture_specular");
    }

    return msh;
}
void load_material_textures(texture_t* tex_arr, struct aiMaterial* mat, enum aiTextureType type, char* typeName){
    for(unsigned int i = 0; i < aiGetMaterialTextureCount(mat,type); i++){
        struct aiString str;
        enum aiReturn rtrn = aiGetMaterialTexture(mat,type,i,&str,NULL,NULL,NULL,NULL,NULL,NULL);
        if(rtrn == aiReturn_SUCCESS){
            printf("success loading texture\n");
            texture_t texture = init_texture(str.data);
            array_push(tex_arr,texture);
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