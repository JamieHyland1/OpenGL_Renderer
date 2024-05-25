#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "camera.h"

///////////////////////////////////
// This class handles camera movement
// I will probably rewrite this file at 
// some point, I dont particularly like 
// cglm and how is uses arrays for its properties instead of structs
// but for now it will do >:(
///////////////////////////////////


camera_t camera = {
    .position = {0,0,3.0},
    .direction = {0,0,1.0f},
    .cam_vel = {0.0f,0.0f,0.0f},
    .right = {1.0,0.0,0.0},
    .worldUp = {0.0,1.0,0.0},
    .pitch = 0.0,
    .yaw = -90.0,
    .roll = 0.0
};

float camSpeed = 2.0f;
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
void process_mouse_move(float xPos, float yPos,float delta_time){
    float sensitivity = 0.1f;

    camera.yaw += (float)-xPos * sensitivity * delta_time;
    camera.pitch += (float)yPos * sensitivity * delta_time;
    
    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;
        
    updateCameraVectors(delta_time);
}
void process_keyboard_movement(SDL_Event event, float delta_time){
    if(event.key.keysym.sym == SDLK_w){
        glm_vec3_scale(&camera.direction[0],camSpeed*delta_time,&camera.cam_vel[0]);
        glm_vec3_add(&camera.position[0],&camera.cam_vel[0],&camera.position[0]);
        camera.position[1] = 1;
    }
    else if(event.key.keysym.sym == SDLK_s){
        glm_vec3_scale(&camera.direction[0],camSpeed*delta_time,&camera.cam_vel[0]);
        glm_vec3_sub(&camera.position[0],&camera.cam_vel[0],&camera.position[0]);
        camera.position[1] = 1;
    }
    else if(event.key.keysym.sym == SDLK_a){
        glm_vec3_cross(&camera.direction[0],&camera.up[0],&camera.cam_vel[0]);
        glm_vec3_normalize(&camera.cam_vel[0]);
        glm_vec3_scale(&camera.cam_vel[0],camSpeed*delta_time,&camera.cam_vel[0]);
        glm_vec3_sub(&camera.position[0],&camera.cam_vel[0],&camera.position[0]);
        camera.position[1] = 1;
    }
    else if(event.key.keysym.sym == SDLK_d){
        glm_vec3_cross(&camera.direction[0],&camera.up[0],&camera.cam_vel[0]);
        glm_vec3_normalize(&camera.cam_vel[0]);
        glm_vec3_scale(&camera.cam_vel[0],camSpeed*delta_time,&camera.cam_vel[0]);
        glm_vec3_add(&camera.position[0],&camera.cam_vel[0],&camera.position[0]);
        camera.position[1] = 1;
    }
    updateCameraVectors(delta_time);

}
void camera_look_at(mat4* view){
    vec3 t = {0.0,0.0,0.0};
    glm_vec3_add(&camera.position[0],&camera.direction[0],&t[0]);
    glm_lookat_rh_no(&camera.position[0],&t[0],&camera.up[0], view[0]);
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