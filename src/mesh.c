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

void draw_mesh(mesh_t* mesh, shader_t* shader) {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    int numTextures = array_length(mesh->textures);

    for (int i = 0; i < numTextures; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        char final_name[256]; // Buffer to hold the final uniform name

        if (strstr(mesh->textures[i].type, "texture_diffuse") != NULL) {
            snprintf(final_name, sizeof(final_name), "material.texture_diffuse%d", diffuseNr++);
        } else if (strstr(mesh->textures[i].type, "texture_specular") != NULL) {
            snprintf(final_name, sizeof(final_name), "material.texture_specular%d", specularNr++);
        } else {
            // Handle other types of textures if any
            continue;
        }

        set_int(shader->shader_ID, final_name, i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, array_length(mesh->indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}