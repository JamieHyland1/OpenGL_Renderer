#define GLEW_STATIC
#define CGLM_STRUCT_API_NS
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "windows.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "../include/headers/core.h"
#define NUM_SHADERS 2

bool mouse_button_down = false;
shader_t shaders[NUM_SHADERS];
float *dist_arr;
float time = 0;
float fov = 45.0f;
mat4 model, view, projection;
float p_angle = 45.0f;
float aspect_ratio;

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and renderer loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
static float delta_time = 0;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
HANDLE shader_handle;

unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
unsigned int quadVAO, quadVBO;
unsigned int cubeVAO, cubeVBO;
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubeTexture;

///////////////////////////////////////////////////////////////////////////////
// Init function for openGL to set its various parameters and load models
///////////////////////////////////////////////////////////////////////////////
int init_openGL()
{
    printf("initialized shaders\n");
    stbi_set_flip_vertically_on_load(false);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            is_running = false;
            break;
        }
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                is_running = false;
            }
            process_keyboard_movement(event, 1);
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_button_down = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_button_down = false;
            break;
        case SDL_MOUSEMOTION:
            if (mouse_button_down)
            {
                process_mouse_move((float)event.motion.xrel, (float)event.motion.yrel, 1);
            }
            break;
        case SDL_MOUSEWHEEL:
            fov -= (float)event.wheel.y;
            if (fov < 1.0f)
                fov = 1.0f;
            if (fov > 45.0f)
                fov = 45.0f;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void)
{
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

    time = (float)SDL_GetTicks() / 1000;

    glm_mat4_identity(model);
    glm_translate(&model[0], (vec3){0.0, 0.0, -10.0});
    glm_rotate_y(&model[0], time * 1.5, &model[0]);

    camera_look_at(&view);
    aspect_ratio = (float)get_window_width() / (float)get_window_height();
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, projection);
    previous_frame_time = SDL_GetTicks();
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void)
{
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.243f, 0.243f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}
///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void)
{
}
///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *args[])
{
    setup();
    is_running = init_openGL();
    while (is_running)
    {
        process_input();
        update();
        render();
    }
    free_resources();
    return 0;
}

