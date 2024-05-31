#include <stdio.h>
#include "array.h"
#include "shader.h"
#include "mesh.h"
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include "../include/cglm/struct.h"
#include "string.h"
#include "vertex.h"
#include "texture.h"

void setup_mesh(mesh_t* mesh){
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, array_length(mesh->vertices) * sizeof(vertex_t), &mesh->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_length(mesh->indices) * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, TexCoords));

    glBindVertexArray(0);
}

void draw_mesh(mesh_t* mesh, shader_t* shader){
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    int numTextures = array_length(mesh->textures);
    char* name = NULL;
    char* final_name = NULL;
    for(int i = 0; i < numTextures; i ++){
        //glActiveTexture(GL_TEXTURE0 + i);
        if(strstr(mesh->textures[i].type,"texture_diffuse") != NULL){
            char* str_type = "texture_diffuse";
            int length = snprintf( NULL, 0, "%d", diffuseNr);
            char num[length];
            snprintf(num, length + 1, "%d", diffuseNr);
            name = malloc(strlen(str_type) + length + 1);
            final_name = malloc(sizeof(char) * (strlen("material.") + strlen(str_type) + length + 1) );
            strcpy(name,str_type);
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
            name = malloc(strlen(str_type) + length  + 1);
            final_name = malloc(sizeof(char) * (strlen("material.") + strlen(str_type) + length + 1) );
            strcpy(name,str_type);
            strcat(name,num);
            strcpy(final_name,"material.");
            strcat(final_name,name);
            specularNr++;
        }
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
        set_int(shader->shader_ID,"material.texture_diffuse1",0);
        
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, array_length(mesh->indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    if(final_name != NULL && name != NULL){
        free(final_name);
        free(name);
    }
}