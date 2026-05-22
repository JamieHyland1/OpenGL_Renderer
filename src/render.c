#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "core.h"

///////////////////////////////////////////////////////////////////////////////
// Render scene
///////////////////////////////////////////////////////////////////////////////
void render(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.243f, 0.243f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SDL_GL_SwapWindow(get_window());
}

