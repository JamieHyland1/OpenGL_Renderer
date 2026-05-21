#ifndef display_h
#define display_h
#include <stdbool.h>
#include <SDL.h>

int setup(void);
int get_window_width();
int get_window_height();
void cancel_renderer();
bool get_running_status();
SDL_Window* get_window();

#define FPS 60 
#define FRAME_TARGET_TIME (1000 / FPS)

#endif
