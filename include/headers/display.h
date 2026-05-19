#ifndef display_h
#define display_h

int setup(void);
int get_window_width();
int get_window_height();

#define FPS 60 
#define FRAME_TARGET_TIME (1000 / FPS)

#endif
