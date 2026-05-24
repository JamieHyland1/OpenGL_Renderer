#ifndef camera_h
#define camera_h
#include "../include/GL/glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
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

extern camera_t camera;

void init_camera(vec3 position, vec3 w_up);
void camera_look_at(mat4 view);
void process_mouse_move(float xPos, float yPos, float delta_time);
void update_camera_movement(float delta_time);
void updateCameraVectors(float delta_time);
void get_camera_position(vec3* vec);
void get_camera_direction(vec3* vec);
void rotate_camera_y(float delta);
void rotate_around_point(vec3 target, float radius, float angle, mat4* view);
void set_camera_rotating(bool rotating);
bool get_is_camera_rotating();
void set_camera_viewport_hovered(bool hovered);
bool get_is_camera_viewport_hovered();
#endif
