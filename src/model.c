#include <errno.h>   
#include <string.h>  
#include "../include/assimp/scene.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/postprocess.h"
#include "../include/assimp/material.h"
#include "../include/headers/stb_image.h"
#include "../include/headers/core.h"


static void load_material_textures(mesh_t* mesh, struct aiMaterial* mat, enum aiTextureType type, char* typeName, char* directory){
    int texture_count = aiGetMaterialTextureCount(mat,type);
    mesh->textures = malloc(sizeof(texture_t) * texture_count);
    for(unsigned int i = 0; i < texture_count; i++){
        struct aiString str;
        enum aiReturn rtrn = aiGetMaterialTexture(mat,type,i,&str,NULL,NULL,NULL,NULL,NULL,NULL);
        if(rtrn == aiReturn_SUCCESS){
            texture_t texture = init_texture(str.data);
            texture.type = typeName;
            mesh->textures[i] = texture;
        }
        else if(rtrn == aiReturn_FAILURE){
            printf("error loading texture data\n");
        }
    }
}


static mesh_t process_mesh(struct aiMesh* aimesh, const struct aiScene* scene, char* directory){
    mesh_t mesh = {0};
    int N_VERTS = aimesh->mNumVertices;
    errno = 0;
    mesh.vertices = malloc(sizeof(vertex_t) * N_VERTS);
    if(mesh.vertices == NULL){
        fprintf(stderr, "malloc failed allocating vertices for mesh: %s\n", strerror(errno));
        return mesh;
    }


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
        
        mesh.vertices[i] = vertex;        
    }

    for(unsigned int i = 0; i < aimesh->mNumFaces; i ++){
        struct aiFace face =  aimesh->mFaces[i];
        errno = 0;
        mesh.indices = malloc(sizeof(int) * face.mNumIndices);
        if(mesh.indices == NULL){
            fprintf(stderr, "malloc failed allocating vertices for mesh: %s\n", strerror(errno));
        }

        for(unsigned int j = 0; j < face.mNumIndices; j++){
           mesh.indices[i] = face.mIndices;
        }
    }

    if(aimesh->mMaterialIndex >= 0){
        struct aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        load_material_textures(&mesh, material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
        load_material_textures(&mesh, material, aiTextureType_SPECULAR,"texture_specular",directory);
    }

    return mesh;
}


static void process_node(model_t* model, struct aiNode* node, const struct aiScene* scene){
    int numMeshes = node->mNumMeshes;
    model->num_meshes += numMeshes;//remember when initializing the model to set this to 0 originally or it might contain garbage
    errno = 0;
    model->meshes = malloc(sizeof(mesh_t) * numMeshes);
    if(model->meshes == NULL){
        fprintf(stderr, "malloc failed allocating memory for number of meshes: %s\n", strerror(errno));
        return;
    }
    for(unsigned int i = 0; i < numMeshes; i++){
        struct aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]]; 
        mesh_t mesh;
        mesh = process_mesh(aimesh, scene, model->directory);
        model->meshes[i] = mesh;
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i ++){
        process_node(model,node->mChildren[i],scene);
    }
}


bool load_model(model_t* model, char* path){
    const struct aiScene* scene = aiImportFile(path, (aiProcess_Triangulate));
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return false;
    }

    char* directory = strdup(path);
    char* last_slash = strrchr(directory, '/');
    if (last_slash != NULL) {
        last_slash[1] = '\0';
    }
    model->directory = strdup(directory);
    process_node(model, scene->mRootNode, scene);
    return true;
}


void draw_model(model_t* model,shader_t* shader){
    int numMeshes = array_length(model->meshes);
    for(unsigned int i = 0; i < numMeshes; i++){
        draw_mesh(&model->meshes[i],shader);
    }
}


void free_model(model_t* model){
    /* TODO: free model resources */
}
