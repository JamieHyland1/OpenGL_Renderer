#ifndef texture_h
#define texture_h
#include "stb_image.h"
#include "../include/GL/glew.h"
typedef struct{
    GLuint id;
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
}texture_t;

texture_t init_texture(const char* filename);


#endif