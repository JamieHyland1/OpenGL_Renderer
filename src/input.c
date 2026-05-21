#include <SDL.h>
#include <stdbool.h>

#include "core.h"

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            cancel_renderer();
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
              cancel_renderer();
            }
            process_keyboard_movement(event, 1);
            break;

        case SDL_MOUSEBUTTONDOWN:
            break;

        case SDL_MOUSEBUTTONUP:
            break;

        case SDL_MOUSEMOTION:
            /* process_mouse_move((float)event.motion.xrel,
                                  (float)event.motion.yrel, 1); */
            break;

        case SDL_MOUSEWHEEL:
            /* fov -= (float)event.wheel.y;
               if (fov < 1.0f) fov = 1.0f;
               if (fov > 45.0f) fov = 45.0f; */
            break;
        }
    }
}
