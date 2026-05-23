#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "core.h"
#include "cimgui.h"
#include "cimgui_impl.h"
#include <cglm/cglm.h>
#include <cglm/struct.h>

///////////////////////////////////////////////////////////////////////////////
// Render scene UI
///////////////////////////////////////////////////////////////////////////////
static void render_ui(){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();
    igDockSpaceOverViewport(0, igGetMainViewport(),
                            ImGuiDockNodeFlags_PassthruCentralNode, NULL);
    igBegin("Scene Info", NULL, 0);
    /* igSliderFloat("FOV", &fov, 1.0f, 120.0f, "%.1f", 0);
    igSliderFloat3("Position", object.position, -20.0f, 20.0f, "%.2f", 0);
    igSliderFloat3("Scale", object.scale, 0.1f, 5.0f, "%.2f", 0); */
    vec3 cameraPos;
    get_camera_position(&cameraPos);
    igText("Camera pos: x: %f y: %f z: %f",cameraPos[0], cameraPos[1], cameraPos[2]);
    igText("%.1f FPS", igGetIO_Nil()->Framerate);
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
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.243f, 0.243f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    render_objects();
    render_ui();

    SDL_GL_SwapWindow(get_window());
}

