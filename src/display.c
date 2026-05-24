#include <GL/glew.h>
#include "display.h"
#include "camera.h"
#include "stb/stb_image.h"
#include "display.h"
#include "camera.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cimgui.h"

#include "cimgui_impl.h"

static bool is_running = false;

static int window_width = 800;
static int window_height = 600;

static SDL_Window *window = NULL;
SDL_GLContext context = NULL;

static SDL_Renderer *renderer = NULL;
float aspect_ratio = 0.0f;

vec3 cameraPos = {0.0f, 1.0f, 5.0f};
vec3 up = {0.0f, 1.0f, 0.0f};
float delta_time = 0.0f;
GLuint framebuffer = 0;
GLuint textureColorbuffer = 0;
GLuint rbo = 0;
///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////

bool setup(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "The window into Jamie's madness",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "Error creating GL context: %s\n", SDL_GetError());
        return false;
    }
    SDL_GL_MakeCurrent(window, context);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW init failed\n");
        return false;
    }
    glViewport(0, 0, window_width, window_height);

    igCreateContext(NULL);
    ImGuiIO* io = igGetIO_Nil();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 330");
    igStyleColorsDark(NULL);

    init_camera(cameraPos, up);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);   // BIND first

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          window_width, window_height);   // consistent size
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "ERROR: Framebuffer incomplete!\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // back to default

    return true;
}

int get_window_width(){
  return window_width;
}

int get_window_height(){
  return window_height;
}

static void resize_screen(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    window_width  = width;
    window_height = height;
    glViewport(0, 0, width, height);

    float aspect = (float)width / (float)height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 1000.0f, projection);

}
void set_running_status(bool status){
    is_running =  status;
}

void cancel_renderer(){
    is_running = false;
}

bool get_running_status(){
    return is_running;
}

SDL_Window* get_window(){
    return window;
}

void check_window_resize(void) {
    SDL_Window* window = get_window();

    int w = 0;
    int h = 0;

    SDL_GL_GetDrawableSize(window, &w, &h);

    if (w != get_window_width() || h != get_window_height()) {
        resize_screen(w, h);
    }
}
