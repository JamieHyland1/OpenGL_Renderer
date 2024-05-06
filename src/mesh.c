#include <stdio.h>
#include "array.h"
#include "shader.h"
#include "mesh.h"
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include "string.h"
#include "vertex.h"
#include "texture.h"

void load_mesh_obj_data(mesh_t* mesh, char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    char line[1024];
    
    while (fgets(line, 1024, file)) {
        vertex_t currentVertex;
        if (strncmp(line, "v ", 2) == 0) {
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            glm_vec3_copy(vertex,currentVertex.position);
        }
        if (strncmp(line, "vt ", 3) == 0) {
            vec2 texCoord;
            sscanf(line, "vt %f %f %f", &texCoord[0], &texCoord[1], &texCoord[2]);
            glm_vec2_copy(texCoord,currentVertex.TexCoords);
        }
        if (strncmp(line, "vn ", 3) == 0) {
            vec3 normal;
            sscanf(line, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
            glm_vec3_copy(normal,currentVertex.Normal);
        }
        if(strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0], 
                &vertex_indices[1], &texture_indices[1], &normal_indices[1], 
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );  

            array_push(mesh->indices,vertex_indices[0]);
            array_push(mesh->indices,vertex_indices[1]);
            array_push(mesh->indices,vertex_indices[2]);
        }

        array_push(mesh->vertices, currentVertex);
    }
    
}

void setup_mesh(mesh_t* mesh){
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, array_length(mesh->vertices) * sizeof(vertex_t), &mesh->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_length(mesh->indices) * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(vertex_t), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, Normal));

    glBindVertexArray(0);
}

void draw_mesh(mesh_t* mesh, shader_t* shader){

    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    int numTextures = array_length(mesh->textures);
    //printf("num textures: %d", numTextures);
    //printf("num textures: %s", mesh->textures[0].type);
    char* name;
    char* final_name;
    for(int i = 0; i < numTextures; i ++){
        glActiveTexture(GL_TEXTURE0 + i);
        if(strstr(mesh->textures[i].type,"texture_diffuse") != NULL){
            char* str_type = "texture_diffuse";
            int length = snprintf( NULL, 0, "%d", diffuseNr);
            char num[length];
            snprintf(num, length + 1, "%d", diffuseNr);
            name = malloc(strlen(str_type) + length + strlen("_") + 1);
            final_name = malloc(strlen(name) + strlen("material.") + 1);
            strcpy(name,str_type);
            strcat(name,"_");
            strcat(name,num);
            
            strcpy(final_name,"material.");
            strcat(final_name,name);
            diffuseNr++;
        }
        else if(strstr(mesh->textures[i].type,"texture_specular") == NULL){
            char* str_type = "texture_specular";
            int length = snprintf( NULL, 0, "%d", diffuseNr);
            char num[length];
            snprintf(num, length + 1, "%d", diffuseNr);
            name = malloc(strlen(str_type) + length + strlen("_") + 1);
            final_name = malloc(strlen(name) + strlen("material.") + 1);
            strcpy(name,str_type);
            strcat(name,"_");
            strcat(name,num);
            
            strcpy(final_name,"material.");
            strcat(final_name,name);
            specularNr++;
        }
        
        set_int(shader->shader_ID,final_name,i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    free(final_name);
    free(name);
}