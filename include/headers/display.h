#ifndef display_h
#define display_h
#include <stdbool.h>
#include <SDL2/SDL.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>
bool setup(void);
int get_window_width();
int get_window_height();
void check_window_resize(void);
void set_running_status(bool status);
void cancel_renderer();
bool get_running_status();
SDL_Window* get_window();

#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)
extern mat4 model, view, projection;
extern float fov; 
extern float aspect_ratio;
extern float delta_time;

#endif
