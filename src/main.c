#define GLEW_STATIC
#define CGLM_STRUCT_API_NS
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include "../include/cglm/struct.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/postprocess.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "shader.h"
#include "array.h"
#include "texture.h"
#include "camera.h"
#include "windows.h"
#include "material.h"
#include "fileapi.h"
#include "synchapi.h"
#include "pthread.h"
#include "mesh.h"
#include "model.h"
#define FPS 60
#define FRAME_TARGET_TIME  (1000 / FPS)
#define NUM_SHADERS 2

//TODO: clean up all these variables into a display file or some sort of file that handles the settings of the renderer
static int window_width = 800;
static int window_height = 600;
bool mouse_button_down = false;

DWORD shaderStatus;
Shader_Type frag = FRAGMENT;
Shader_Type vert = VERTEX;
vec3 cameraPos = {0.0f, 0.0f, 5.0f};
vec3 up = {0.0f,1.0f,0.0f};
shader_t shader;
float time = 0;
float fov = 45.0f;
mat4 model,view,projection;
shader_t error_shader;
shader_t shaders[NUM_SHADERS];
HANDLE dwChangeHandles, files; 
model_t cubeModel;
OVERLAPPED  overlapped = {0};
float p_angle = 45.0f;
BYTE notifBuffer[4096];
texture_t test;
///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and renderer loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
static float delta_time = 0; 

static SDL_Window* window = NULL;
SDL_GLContext context = NULL;
static SDL_Renderer* renderer = NULL;
HANDLE shader_handle;

///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////

int setup(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    fprintf(stderr, "Error initializing SDL window");
    return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode displayMode;

    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    // set_material_ambient(&mat, (vec3){1.0f, 0.5f, 0.31f});
    // set_material_diffuse(&mat, (vec3){1.0f, 0.5f, 0.31f});
    // set_material_specular(&mat,(vec3){0.5f, 0.5f, 0.5f});
    // set_material_shininess(&mat, 32.0f);
    
    
    // printf("number of cube meshes: %d\n", array_length(cubeModel.meshes));

    // int full_screen_width = displayMode.w;
    // int full_screen_height = displayMode.h;
    // window_width = full_screen_width;
    // window_height = full_screen_height;


    stbi_set_flip_vertically_on_load(true);

    window = SDL_CreateWindow(
        "The window into Jamie's madness",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_OPENGL
    );
    if(!window){
        fprintf(stderr, "Error creating SDL window");
        return false;
    }
    renderer = SDL_CreateRenderer(window,-1, 0);

    if(!renderer){
        fprintf(stderr, "Error creating SDL renderer");
        return false;
    }

    SDL_SetWindowBordered(window,SDL_WINDOWPOS_CENTERED);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,context);
    glewExperimental = GL_TRUE;
    
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    init_camera(cameraPos,up);

    return true; 
}
///////////////////////////////////////////////////////////////////////////////
// Init function for openGL to set its various parameters and load models
///////////////////////////////////////////////////////////////////////////////
int init_openGL(){
    printf("initialized shaders\n");
    load_model(&cubeModel,"./Res/Cube/Model/cube.obj");
    
    init_shader(&error_shader, "./shaders/ERROR_FRAG.glsl",   frag);
    init_shader(&error_shader, "./shaders/ERROR_VERTEX.glsl", vert);
    link_shader(&error_shader);

    if(!init_shader(&shader, "./Res/Skull/Shader/obj_vert.glsl", vert)){
        printf("error intialising %s\n", "vertex shader");
        load_error_shader(&shader,&error_shader);
    }
    if(!init_shader(&shader, "./Res/Skull/Shader/obj_frag.glsl", frag)){
        printf("error intialising %s\n", "frag shader");
        load_error_shader(&shader,&error_shader);
    }
    link_shader(&shader);
    int numMeshes = array_length(cubeModel.meshes);
    for(int i = 0; i < numMeshes; i++){
        setup_mesh(&cubeModel.meshes[i]);
    }
    glEnable(GL_DEPTH_TEST); 
    stbi_set_flip_vertically_on_load(true);
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                is_running = false;
                break;
            }
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    is_running = false;
                }
                process_keyboard_movement(event,1);
                break;    
            case SDL_MOUSEBUTTONDOWN:
                mouse_button_down = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_button_down = false;
                break;
            case SDL_MOUSEMOTION:
                    if(mouse_button_down){
                        process_mouse_move((float)event.motion.xrel,(float)event.motion.yrel,1);
                    }
                break;
            case SDL_MOUSEWHEEL:
                fov -= (float) event.wheel.y;
                if (fov < 1.0f)
                    fov = 1.0f;
                if (fov > 45.0f)
                    fov = 45.0f; 
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    // Get a delta time factor converted to seconds to be used to update our game objects
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

    time = (float)SDL_GetTicks()/1000;

    glm_mat4_identity(model);
    glm_rotate_y(&model[0],sin(time) * 5, &model[0]);
    camera_look_at(&view);
    glm_perspective(p_angle,(float)(window_width/window_height),0.1f,100.0f,projection);
    previous_frame_time = SDL_GetTicks();
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.0f,0.2f,0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);

    use_shader(shader.shader_ID);
    set_float(shader.shader_ID,"time",time);
    set_float(shader.shader_ID,"material.t",time);
    set_matrix(shader.shader_ID,"model", model);
    set_matrix(shader.shader_ID,"view", view);
    set_matrix(shader.shader_ID,"projection", projection);
    draw_model(&cubeModel,&shader);

    SDL_GL_SwapWindow(window);
}
///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {

}
///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]){
    setup();
    is_running = init_openGL();
    glViewport(0, 0, window_width, window_height);
    poll_shaders();
    while (is_running) {
        WaitForSingleObjectEx(shader_handle,0,TRUE); // check to see if any shaders have been changed
        process_input();
        update();
        render();
    }
    free_resources();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Poll shader directory for changes to shaders and reload if necessary
///////////////////////////////////////////////////////////////////////////////
void CALLBACK ShaderChangedCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    if(dwErrorCode == ERROR_SUCCESS){
        FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)notifBuffer;
        if(pNotify->Action == FILE_ACTION_MODIFIED){
            size_t combinedLength = strlen("./shaders/") + wcslen(pNotify->FileName) + 1; // +1 for null terminator
            char* path = (char*)malloc(combinedLength);
            strcpy(path, "./shaders/");
            char *t = malloc((wcslen(pNotify->FileName) + 1) * sizeof(char));
            wcstombs(t, pNotify->FileName, 128);
            strcat(path,t);
            for(int i = 0; i < NUM_SHADERS; i++){
                if(strcmp(path,shaders[i].fragment_source)){
                    if(!reload_shader(&shaders[i])){
                        printf("Error updating shader, fallback to error shader\n");
                        load_error_shader(&shaders[i],&error_shader);
                    } 
                }
            }
            free(t);
            free(path);
            
        }
    }else {
        printf("Error occurred: %ld\n", dwErrorCode);
    }
    CloseHandle(shader_handle);
    CloseHandle(overlapped.hEvent);
    poll_shaders();
}

//TODO: ADD SUPPORT FOR MAC/Linux
void poll_shaders(){
    
        shader_handle = CreateFileA("./shaders/",(GENERIC_READ | GENERIC_WRITE),(FILE_SHARE_READ | FILE_SHARE_READ),NULL, ( OPEN_EXISTING) ,FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
        if(shader_handle == INVALID_HANDLE_VALUE){
            printf("Error: Unable to open directory.\n");
            return;
        }
        overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (overlapped.hEvent == NULL) {
            printf("Error creating event object.\n");
            CloseHandle(shader_handle);
            return;
        }
        bool c = ReadDirectoryChangesW(shader_handle, notifBuffer, sizeof(notifBuffer),FALSE,(FILE_NOTIFY_CHANGE_LAST_WRITE), NULL, &overlapped, &ShaderChangedCallback);
        if(!c){
            printf("Error initiating asynchronous directory monitor.\n");
            CloseHandle(overlapped.hEvent);
            CloseHandle(shader_handle);
        }
}