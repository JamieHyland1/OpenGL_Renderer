#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <C:\SDL2\include\SDL.h>
#include "texture.h"
#include "string.h"
#include "array.h"
///////////////////////////////////
// This class handles textures
// specifically loading texture data  
// using stbi library
///////////////////////////////////

//TODO: look into why setting mipmaps for texture causes seg fault :(
    const char* faces[] = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    
    int numFaces = sizeof(faces) / sizeof(faces[0]);
texture_t init_texture(const char* filename){
    texture_t texture;
    glGenTextures(1,&texture.id);
    glBindTexture(GL_TEXTURE_2D,texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



    unsigned char *data = stbi_load(filename,&texture.width,&texture.height,&texture.nrChannels,0);
    if(data && strstr(filename,".png") != NULL){
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, texture.width,texture.height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }else if(data){
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, texture.width,texture.height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    }else{
        printf("Failed to load texture :( ");
    }

    stbi_image_free(data);
    
    return texture;
}

unsigned int init_cubemap(const char* filepath){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < numFaces; i++) {
        char* path_to_image = malloc(strlen(filepath) + strlen(faces[i]) + 1);
        if (!path_to_image) {
            printf("Failed to allocate memory for path_to_image\n");
            continue;
        }
        strcpy(path_to_image, filepath);
        strcat(path_to_image, faces[i]);

        unsigned char *data = stbi_load(path_to_image, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            printf("Cubemap texture failed to load at path: %s\n", path_to_image);
        }

        free(path_to_image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}