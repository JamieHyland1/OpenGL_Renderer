#ifndef framebuffer_h
#define framebuffer_h
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>

typedef struct{

  GLuint fbo;
  GLuint texture;
  GLuint rbo;
  int width;
  int height;

}framebuffer_t;

extern framebuffer_t fb;

bool init_framebuffer(framebuffer_t* fb, int width, int height);
void resise_framebuffer(framebuffer_t* fb, int width, int height);
void bind_framebuffer(framebuffer_t* fb);
void unbind_framebuffer(framebuffer_t* fb);
void free_framebuffer(framebuffer_t* fb);

#endif
