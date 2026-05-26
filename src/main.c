#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stb/stb_image.h"
#include "../include/headers/framebuffer.h"
#include "../include/headers/skybox.h"
#include "../include/headers/render.h"
#include "../include/headers/object.h"
#include "../include/headers/display.h"
#include "../include/headers/input.h"
#include "../include/headers/camera.h"

// Forward declaration
bool init_openGL(void);
const char* faces[6] = {
    "textures/Yokohama/right.jpg",
    "textures/Yokohama/left.jpg",
    "textures/Yokohama/top.jpg",
    "textures/Yokohama/bottom.jpg",
    "textures/Yokohama/front.jpg",
    "textures/Yokohama/back.jpg"
};
///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
float elapsed_time = 0.0f;
float fov = 45.0f;
float p_angle = 45.0f;
mat4 model, view, projection;

bool is_running = false;
int  previous_frame_time = 0;
///////////////////////////////////////////////////////////////////////////////
// Initialize OpenGL state and load any startup resources
///////////////////////////////////////////////////////////////////////////////
bool init_openGL(void)
{
    printf("initialized shaders\n");

    stbi_set_flip_vertically_on_load(true);
    init_skybox(&skybox, faces, "shaders/skybox_vert.glsl",
                                 "shaders/skybox_frag.glsl");

    object_t* object = &objects[current_num_objects];
    bool init_obj =  init_object(object, "Models/Cube/cube.obj",
                  "shaders/obj_vert.glsl",
                   "shaders/obj_frag_diffuse.glsl");


    object_t* object2 = &objects[current_num_objects];
    init_obj = init_obj &&  init_object(object2, "Models/Skull/skull.obj",
                  "shaders/obj_vert.glsl",
                   "shaders/obj_frag_diffuse.glsl");

    object_t* object3 = &objects[current_num_objects];
    init_obj = init_obj &&  init_object(object3, "Models/Dragon/Dragon.obj",
                  "shaders/obj_vert.glsl",
                   "shaders/obj_frag_diffuse.glsl");

    

    glm_vec3_zero(object->position);
    glm_vec3_zero(object->rotation);
    glm_vec3_one(object->scale);            
    glm_mat4_identity(object->transform);


    glm_vec3_one(object2->position);
    glm_vec3_zero(object2->rotation);
    glm_vec3_one(object2->scale);            
    glm_mat4_identity(object2->transform);

    glm_vec3_one(object3->position);
    glm_vec3_zero(object3->rotation);
    glm_vec3_one(object3->scale);
    glm_mat4_identity(object3->transform);

    return init_obj;
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
    check_window_resize();
    Uint32 now = SDL_GetTicks();
    delta_time = (now - previous_frame_time) / 1000.0f;
    elapsed_time += delta_time;
    previous_frame_time = now;

    for (int i = 0; i < current_num_objects; i++) {
        object_t* object = &objects[i];

        glm_mat4_identity(object->transform);

        glm_translate(object->transform, object->position);

        glm_rotate_x(object->transform, object->rotation[0], object->transform);
        glm_rotate_y(object->transform, object->rotation[1], object->transform);
        glm_rotate_z(object->transform, object->rotation[2], object->transform);

        glm_scale(object->transform, object->scale);
    }

    update_camera_movement(delta_time);
    camera_look_at(view);
    aspect_ratio = (float)fb.width / (float)fb.height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 1000.0f, projection);

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
    set_running_status(setup() && init_openGL());
    while (get_running_status()) {
        process_input();
        update();
        render();
    }
    
    free_resources();
    return 0;
}
