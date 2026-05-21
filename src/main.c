#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "core.h"
#include "stb/stb_image.h"

// Forward declaration
bool init_openGL(void);

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
float elapsed_time = 0.0f;
float fov = 45.0f;
float p_angle = 45.0f;
float aspect_ratio = 0.0f;
mat4 model, view, projection;

bool is_running = false;
int  previous_frame_time = 0;
static float delta_time = 0.0f;

///////////////////////////////////////////////////////////////////////////////
// Initialize OpenGL state and load any startup resources
///////////////////////////////////////////////////////////////////////////////
bool init_openGL(void)
{
    printf("initialized shaders\n");
    stbi_set_flip_vertically_on_load(false);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Update function (fixed timestep)
///////////////////////////////////////////////////////////////////////////////
void update(void)
{
     Uint32 elapsed_since_last = SDL_GetTicks() - previous_frame_time;
    
    if (elapsed_since_last < FRAME_TARGET_TIME) {
        SDL_Delay(FRAME_TARGET_TIME - elapsed_since_last);
    }
    
    Uint32 now = SDL_GetTicks();
    delta_time = (now - previous_frame_time) / 1000.0f;
    elapsed_time += delta_time;  // accumulate to avoid float precision loss
    previous_frame_time = now;    

    glm_mat4_identity(model);
    glm_translate(model, (vec3){0.0f, 0.0f, -10.0f});
    glm_rotate_y(model, elapsed_time * 1.5f, model);
    
    aspect_ratio = (float)get_window_width() / (float)get_window_height();
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, projection);
    
}

///////////////////////////////////////////////////////////////////////////////
// Free dynamically allocated resources
///////////////////////////////////////////////////////////////////////////////
void free_resources(void)
{
    // TODO: free meshes, shaders, textures, etc.
}

///////////////////////////////////////////////////////////////////////////////
// Entry point
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *args[])
{
    (void)argc;  // silence unused warning
    (void)args;
    
    setup();
    is_running = init_openGL();
    
    while (is_running) {
        process_input();
        update();
        render();
    }
    
    free_resources();
    return 0;
}
