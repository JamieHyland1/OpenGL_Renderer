#include <SDL2/SDL.h>
#include "../include/headers/framebuffer.h"
#include "../include/headers/display.h"
framebuffer_t fb = {0};


bool init_framebuffer(framebuffer_t *fb, int width, int height){

    fb->width = width;
    fb->height = height;

    
    glGenFramebuffers(1, &fb->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);   

    glGenTextures(1, &fb->texture);
    glBindTexture(GL_TEXTURE_2D, fb->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fb->width, fb->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, fb->texture, 0);

    glGenRenderbuffers(1, &fb->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          fb->width, fb->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, fb->rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "ERROR: Framebuffer incomplete!\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return true;
}

#include <stdlib.h>   // for abs

void resize_framebuffer(framebuffer_t *fb, int width, int height){
    if (width <= 0 || height <= 0) return;
    if (abs(width - fb->width) < 2 && abs(height - fb->height) < 2) return;  // ignore <2px wobble

    fb->width  = width;
    fb->height = height;

    glBindTexture(GL_TEXTURE_2D, fb->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}
void bind_framebuffer(framebuffer_t *fb){

    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);   
    glViewport(0, 0, fb->width, fb->height);   
    
}

void unbind_framebuffer(framebuffer_t *fb){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, get_window_width(), get_window_height());   

  }

void free_framebuffer(framebuffer_t *fb){

}
