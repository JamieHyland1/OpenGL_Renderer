#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/headers/display.h"
#include "../include/headers/camera.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static bool is_running = false;

static int window_width = 800;
static int window_height = 600;

static SDL_Window *window = NULL;
SDL_GLContext context = NULL;
static SDL_Renderer *renderer = NULL;

vec3 cameraPos = {0.0f, 1.0f, 5.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////

int setup(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL window");
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    printf("window width: %d window height: %d\n", window_width, window_height);
    stbi_set_flip_vertically_on_load(true);

    window = SDL_CreateWindow(
        "The window into Jamie's madness",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer");
        return false;
    }

    SDL_SetWindowBordered(window, SDL_WINDOWPOS_CENTERED);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    glewExperimental = GL_TRUE;
    glViewport(0, 0, window_width, window_height); // Set the viewport to the size of the framebuffer
    glewInit();
    glPolygonMode(GL_BACK, GL_FILL);
    init_camera(cameraPos, up);

    return true;
}

int get_window_width(){
  return window_width;
}

int get_window_height(){
  return window_height;
}
