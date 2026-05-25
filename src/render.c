#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "core.h"
#include "cimgui.h"
#include "cimgui_impl.h"
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "../include/headers/framebuffer.h"
static int selected_object = -1;
///////////////////////////////////////////////////////////////////////////////
// Render scene UI
///////////////////////////////////////////////////////////////////////////////

static void render_ui(void) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();
    igDockSpaceOverViewport(0, igGetMainViewport(),
                            ImGuiDockNodeFlags_PassthruCentralNode, NULL);

    igBegin("Scene Info", NULL, 0);
    igText("%.1f FPS", igGetIO_Nil()->Framerate);
    igEnd();

    igBegin("Scene View (FBO)", NULL, 0);
    {
        ImVec2_c avail = igGetContentRegionAvail();

        ImTextureRef_c tex_ref = { ._TexID = (ImTextureID)fb.texture};

        igImage(
            tex_ref,
            avail,
            (ImVec2_c){0.0f, 1.0f},
            (ImVec2_c){1.0f, 0.0f}
        );
    }
    igEnd();

    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

///////////////////////////////////////////////////////////////////////////////
// Render objects in the scene
///////////////////////////////////////////////////////////////////////////////
static void render_objects(){
    for(int i = 0; i < current_num_objects; i++){
        object_t* object = &objects[i];
        draw_object(object);
    }
    draw_skybox(&skybox, view, projection);
}


///////////////////////////////////////////////////////////////////////////////
// Render scene - thin orchestrator for UI and Scene Objects
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    printf("fbo: %d\n", fb.fbo);
    printf("texture: %d\n", fb.texture);
    printf("rbo: %d\n", fb.rbo);
    printf("width: %d\n", fb.width);
    printf("height: %d\n", fb.height);
    // ---- PASS 1: scene into the FBO (for the ImGui Scene panel) ----
    bind_framebuffer(&fb);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);   // magenta
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    render_objects();
    unbind_framebuffer(&fb);

    // ---- PASS 2: scene to the screen (temporary - delete once panel works) ----
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.243f, 0.243f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    render_objects();

    // ---- UI ----
    render_ui();

    SDL_GL_SwapWindow(get_window());
}
