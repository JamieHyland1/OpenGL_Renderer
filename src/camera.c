#include "../include/GL/glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdint.h>
#include <stdbool.h>
#include "cimgui.h"
#include "cimgui_impl.h"
#include "../include/headers/camera.h"

camera_t camera = {
    .position = {0.0f, 0.0f, 3.0f},
    .direction = {0.0f, 0.0f, -1.0f},
    .cam_vel = {0.0f, 0.0f, 0.0f},
    .right = {1.0f, 0.0f, 0.0f},
    .worldUp = {0.0f, 1.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .pitch = 0.0f,
    .yaw = -90.0f,
    .roll = 0.0f
};



float camSpeed = 12.0f;
static bool camera_is_rotating = false;
static bool camera_viewport_hovered = false;
static float mouse_sensitivity = 0.1f;


void init_camera(vec3 position, vec3 w_up){
    //set the camera position and world up vector
    glm_vec3_copy(position,camera.position);
   // glm_vec3_copy(w_up,camera.worldUp);

    //Get the right vector
    glm_cross(&camera.direction[0],&camera.worldUp[0],&camera.right[0]);
    glm_vec3_normalize(&camera.right[0]);

    glm_vec3_cross(&camera.right[0],&camera.direction[0],&camera.up[0]);

    updateCameraVectors(1);

}

void process_mouse_move(float xrel, float yrel, float delta_time) {
    (void)delta_time;

    camera.yaw += xrel * mouse_sensitivity;
    camera.pitch -= yrel * mouse_sensitivity;

    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    }

    if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }

    updateCameraVectors(delta_time);
}

void update_camera_movement(float delta_time) {
    if (igGetIO_Nil()->WantCaptureKeyboard) return;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    float speed = camSpeed * delta_time;
    vec3 move;

    if (keys[SDL_SCANCODE_W]) {
        glm_vec3_scale(camera.direction, speed, move);
        glm_vec3_add(camera.position, move, camera.position);
    }
    if (keys[SDL_SCANCODE_S]) {
        glm_vec3_scale(camera.direction, speed, move);
        glm_vec3_sub(camera.position, move, camera.position);
    }
    if (keys[SDL_SCANCODE_A]) {
        glm_vec3_cross(camera.direction, camera.up, move);
        glm_vec3_normalize(move);
        glm_vec3_scale(move, speed, move);
        glm_vec3_sub(camera.position, move, camera.position);
    }
    if (keys[SDL_SCANCODE_D]) {
        glm_vec3_cross(camera.direction, camera.up, move);
        glm_vec3_normalize(move);
        glm_vec3_scale(move, speed, move);
        glm_vec3_add(camera.position, move, camera.position);
    }

    updateCameraVectors(delta_time);
}
void camera_look_at(mat4 view) {
    vec3 t;
    glm_vec3_add(camera.position, camera.direction, t);          
    glm_lookat_rh_no(camera.position, t, camera.up, view);
}
void updateCameraVectors(float delta_time){
    // calculate the new Front vector
    float local_pitch = camera.pitch;
    float local_yaw = camera.yaw;
    glm_make_rad(&local_pitch);
    glm_make_rad(&local_yaw);

    vec3 front;
    front[0] = cosf(local_yaw) * cosf(local_pitch);
    front[1] = sinf(local_pitch);
    front[2] = sinf(local_yaw) * cosf(local_pitch);
    glm_vec3_copy(front,camera.direction);
    glm_vec3_normalize(&camera.direction[0]);
    // also re-calculate the Right and Up vector
    glm_cross(&camera.direction[0],&camera.worldUp[0],&camera.right[0]);

    glm_vec3_normalize(&camera.right[0]);
    glm_vec3_cross(&camera.right[0],&camera.direction[0],&camera.up[0]);
    glm_vec3_normalize(&camera.up[0]);

    camera.cam_vel[0] = 0;
    camera.cam_vel[1] = 0;
    camera.cam_vel[2] = 0;
    
    
}
void get_camera_position(vec3* vec){
    glm_vec3_copy(&camera.position[0],vec[0]);
}
void get_camera_direction(vec3* vec){
    glm_vec3_copy(&camera.direction[0],vec[0]);
}
void rotate_around_point(vec3 target, float radius, float angle, mat4* view) {
    // Calculate the new camera position on the circular path
    camera.position[0] = target[0] + radius * cosf(angle);
    camera.position[1] = target[1];
    camera.position[2] = target[2] + radius * sinf(angle);

    // Update the camera's direction to always look at the target point
    vec3 direction;
    glm_vec3_sub(target, camera.position, direction);
    glm_vec3_normalize(direction);
    glm_vec3_copy(direction, camera.direction);

    // Update the right and up vectors
    glm_vec3_cross(camera.direction, camera.worldUp, camera.right);
    glm_vec3_normalize(camera.right);
    glm_vec3_cross(camera.right, camera.direction, camera.up);
    glm_vec3_normalize(camera.up);

    // Update the view matrix
    glm_lookat_rh_no(camera.position, target, camera.up, view[0]);
}

void set_camera_rotating(bool rotating){
    camera_is_rotating = rotating;
}

bool get_is_camera_rotating(){
    return camera_is_rotating;
}

void set_camera_viewport_hovered(bool hovered){
    camera_viewport_hovered = hovered;
}

bool get_is_camera_viewport_hovered(){
    return camera_viewport_hovered;
}
