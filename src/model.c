#include <errno.h>   
#include <string.h>  
#include "../include/assimp/scene.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/postprocess.h"
#include "../include/assimp/material.h"
#include "../include/headers/stb_image.h"
#include "../include/headers/core.h"


static void load_material_textures(mesh_t* mesh, struct aiMaterial* mat,
                                   enum aiTextureType type, char* typeName,
                                   char* directory) {
    unsigned int texture_count = aiGetMaterialTextureCount(mat, type);

    if (texture_count == 0) {
        return;  // nothing to load
    }

    mesh->textures = malloc(sizeof(texture_t) * texture_count);
    if (!mesh->textures) {
        fprintf(stderr, "Failed to allocate textures\n");
        return;
    }
    mesh->num_textures = texture_count;   // track the count!

    for (unsigned int i = 0; i < texture_count; i++) {
        struct aiString str;
        enum aiReturn rtrn = aiGetMaterialTexture(mat, type, i, &str,
                                                  NULL, NULL, NULL, NULL, NULL, NULL);
        if (rtrn == aiReturn_SUCCESS) {
            // Prepend the model's directory to the texture filename
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s%s", directory, str.data);

            texture_t texture = init_texture(full_path);
            texture.type = typeName;
            mesh->textures[i] = texture;
        } else {
            fprintf(stderr, "Error loading texture data for %s\n", typeName);
        }
    }
}


static mesh_t process_mesh(struct aiMesh* aimesh, const struct aiScene* scene, char* directory){
    mesh_t mesh = {0};
    mesh.num_vertices = aimesh->mNumVertices;
    errno = 0;
    mesh.vertices = malloc(sizeof(vertex_t) * mesh.num_vertices);
    if(mesh.vertices == NULL){
        fprintf(stderr, "malloc failed allocating vertices for mesh: %s\n", strerror(errno));
        return mesh;
    }


    for(unsigned int i = 0; i < mesh.num_vertices; i++){

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

        size_t total_indices = aimesh->mNumFaces * 3;

        errno = 0;
        mesh.indices = malloc(sizeof(unsigned int) * total_indices);
        if (mesh.indices == NULL) {
            fprintf(stderr, "malloc failed allocating indices for mesh: %s\n", strerror(errno));
            return mesh;
        }
        mesh.num_indices = total_indices;

        // Copy indices from each face
        size_t idx = 0;
        for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
            struct aiFace face = aimesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh.indices[idx++] = face.mIndices[j];
            }
        }

    if(aimesh->mMaterialIndex >= 0){
        struct aiMaterial *material = scene->mMaterials[aimesh->mMaterialIndex];
        load_material_textures(&mesh, material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
        load_material_textures(&mesh, material, aiTextureType_SPECULAR,"texture_specular",directory);
    }

    return mesh;
}


// Pass 1: count all meshes in the entire node tree
static unsigned int count_meshes(struct aiNode* node) {
    unsigned int count = node->mNumMeshes;
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        count += count_meshes(node->mChildren[i]);
    }
    return count;
}

// Pass 2: fill meshes, using a running index passed by pointer
static void process_node(model_t* model, struct aiNode* node,
                         const struct aiScene* scene, unsigned int* index) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        struct aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes[*index] = process_mesh(aimesh, scene, model->directory);
        (*index)++;
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(model, node->mChildren[i], scene, index);
    }
}


bool load_model(model_t* model, const char* path) {
    const struct aiScene* scene = aiImportFile(path, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return false;
    }

    // Derive directory
    char* directory = strdup(path);
    char* last_slash = strrchr(directory, '/');
    if (last_slash != NULL) {
        last_slash[1] = '\0';
    }
    model->directory = strdup(directory);
    free(directory);   // strdup'd twice - free the temp

    // Pass 1: count meshes, allocate the array ONCE
    model->num_meshes = count_meshes(scene->mRootNode);
    model->meshes = malloc(sizeof(mesh_t) * model->num_meshes);
    if (model->meshes == NULL) {
        fprintf(stderr, "malloc failed for meshes\n");
        aiReleaseImport(scene);
        return false;
    }

    // Pass 2: fill the array
    unsigned int index = 0;
    process_node(model, scene->mRootNode, scene, &index);

    // Pass 3: upload each mesh to the GPU
    for (unsigned int i = 0; i < model->num_meshes; i++) {
        setup_mesh(&model->meshes[i]);
    }

    aiReleaseImport(scene);
    return true;
}


void draw_model(model_t* model,shader_t* shader){
    size_t numMeshes = model->num_meshes;
    for(unsigned int i = 0; i < numMeshes; i++){
        draw_mesh(&model->meshes[i],shader);
    }
}


void free_model(model_t* model){
    /* TODO: free model resources */
}
