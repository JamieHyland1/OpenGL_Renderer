#ifndef camera_h
#define camera_h
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    vec3 cam_vel;
    float pitch;
    float yaw;
    float roll;
} camera_t;

void init_camera(vec3 position, vec3 w_up);
void camera_look_at(mat4* view);
void process_mouse_move(float xPos, float yPos, float delta_time);
void process_keyboard_movement(SDL_Event event, float delta_time);
void updateCameraVectors(float delta_time);
#endif