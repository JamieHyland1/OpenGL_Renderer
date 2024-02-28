#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <C:\SDL2\include\SDL.h>
#include "texture.h"
///////////////////////////////////
// This class handles textures
// specifically loading texture data  
// using stbi library
///////////////////////////////////

//TODO: look into why setting mipmaps for texture causes seg fault :(
texture_t init_texture(const char* filename){
    texture_t texture;
    glGenTextures(1,&texture.id);
    glBindTexture(GL_TEXTURE_2D,texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
    unsigned char *data = stbi_load(filename,&texture.width,&texture.height,&texture.nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, texture.width,texture.height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    }else{
        printf("Failed to load texture :( ");
       
    }

  stbi_image_free(data);
  
    return texture;
}