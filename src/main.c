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
shader_t shaders[NUM_SHADERS];
DWORD shaderStatus;
Shader_Type frag = FRAGMENT;
Shader_Type vert = VERTEX;
vec3 cameraPos = {0.0f, 1.0f, 5.0f};
vec3 up = {0.0f,1.0f,0.0f};
model_t container_model;

model_t floor_model;
model_t sphere_model;
shader_t shader;
shader_t post_process_shader;
float* dist_arr;
float time = 0;
float fov = 45.0f;
mat4 model,view,projection;
shader_t error_shader;
shader_t skybox_shader;
shader_t mirror_shader;
bool enable_post_process = false;
HANDLE dwChangeHandles, files; 
model_t cubes_model, floor_model;
OVERLAPPED  overlapped = {0};
float p_angle = 45.0f;
BYTE notifBuffer[4096];
texture_t test;

float aspect_ratio;

float quadVertices[] = {  
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};	
float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

shader_t post_process_shader;
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

unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
unsigned int quadVAO, quadVBO;
unsigned int cubeVAO, cubeVBO;
unsigned int skyboxVAO, skyboxVBO;
unsigned int cubeTexture;

void enable_post_processing(int window_width, int window_height){ 
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 

    // Create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach the color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Attach the renderbuffer object to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if the framebuffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}
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
    printf("window width: %d window height: %d\n",window_width, window_height);
    int full_screen_width = displayMode.w;
    int full_screen_height = displayMode.h;
    window_width = full_screen_width;
    window_height = full_screen_height;

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
    glViewport(0, 0, window_width, window_height);  // Set the viewport to the size of the framebuffer
    glewInit();
    glPolygonMode(GL_BACK, GL_FILL);
    init_camera(cameraPos,up);

    return true; 
}
///////////////////////////////////////////////////////////////////////////////
// Init function for openGL to set its various parameters and load models
///////////////////////////////////////////////////////////////////////////////
int init_openGL(){
    printf("initialized shaders\n");
    init_shader(&error_shader, "./shaders/ERROR_FRAG.glsl",   frag);
    init_shader(&error_shader, "./shaders/ERROR_VERTEX.glsl", vert);
    link_shader(&error_shader);
    //Add models and shaders here:
    init_shader(&shader, "./shaders/obj_vert.glsl", vert);
    init_shader(&shader, "./shaders/obj_frag_diffuse.glsl", frag);
    link_shader(&shader);

    init_shader(&mirror_shader, "./shaders/mirror_vertex.glsl", vert);
    init_shader(&mirror_shader, "./shaders/mirror_frag.glsl", frag);
    link_shader(&mirror_shader);

    init_shader(&post_process_shader, "./shaders/post_process_vert.glsl", vert);
    init_shader(&post_process_shader, "./shaders/post_process_frag.glsl", frag);
    link_shader(&post_process_shader);
   
    init_shader(&skybox_shader, "./shaders/skybox_vert.glsl", vert);
    init_shader(&skybox_shader, "./shaders/skybox_frag.glsl", frag);
    link_shader(&skybox_shader);

    

    load_model(&floor_model,"./Models/Containers/floor.obj");
    load_model(&sphere_model, "./Models/Sphere/Sphere.obj");

    int num_floor_msh  = array_length(floor_model.meshes);
    int num_sphere_msh = array_length(sphere_model.meshes);

    
    for(int i = 0; i <  num_floor_msh; i++){
        setup_mesh(&floor_model.meshes[i]);
    }

    for(int i = 0; i <  num_sphere_msh; i++){
        setup_mesh(&sphere_model.meshes[i]);
    }


    stbi_set_flip_vertically_on_load(false);


    cubeTexture = init_cubemap("./textures/Yokohama/");

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    glEnable(GL_BLEND);     
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);  
    glDepthFunc(GL_LESS); 
    glMatrixMode(GL_PROJECTION);
    enable_post_processing(window_width/2,window_height/2);
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
                else if(event.key.keysym.sym == SDLK_SPACE){
                    enable_post_process = !enable_post_process;
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
    glm_translate(&model[0], (vec3){0.0,0.0,-10.0});
    glm_rotate_y(&model[0] ,time * 1.5, &model[0]);

    camera_look_at(&view);
    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, projection);
    previous_frame_time = SDL_GetTicks();
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    if(enable_post_process){
        // Bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, window_width/2, window_height/2);  // Set the viewport to the size of the framebuffer
        
    }
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    // Clear the buffers
    glClearColor(0.243f, 0.243f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
    
    // Add rendering code here
    glm_mat4_identity(model);
    use_shader(shader.shader_ID);

    set_matrix(shader.shader_ID, "model", model);
    set_matrix(shader.shader_ID, "view", view);
    set_matrix(shader.shader_ID, "projection", projection);

    // draw_model(&floor_model, &shader);


    use_shader(mirror_shader.shader_ID);
     glm_rotate_y(&model[0] ,time *-1.5, &model[0]);
    //glm_translate(&model[0], (vec3){0.0, 1.0, 0.0});
    glm_scale(&model[0],(vec3){4.5,4.5,4.5});
    set_matrix(mirror_shader.shader_ID, "model", model);
    set_matrix(mirror_shader.shader_ID, "view", view);
    set_matrix(mirror_shader.shader_ID, "projection", projection);
    vec3 pos;
    get_camera_position(&pos);
    
    set_vec3(mirror_shader.shader_ID, "cameraPos",&pos[0]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
    draw_model(&sphere_model, &mirror_shader);

    rotate_around_point((vec3){0.0,3.5,0.0},28,time/4,&view);

    draw_skybox();

    if(enable_post_process){
        run_post_processing();
    }

    SDL_GL_SwapWindow(window);
}
///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
// free(framebuffer);
// free(textureColorbuffer);
// free(rbo);
// free(quadVAO);
// free(quadVBO);
// free(cubeVAO);
// free(cubeVBO);
// free(skyboxVAO); 
// free(skyboxVBO);
// free(cubeTexture);
}
///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]){
    setup();
    is_running = init_openGL();
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


void draw_skybox(){
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    use_shader(skybox_shader.shader_ID);
    glm_mat4_identity(view);
    camera_look_at(&view);
   // if(enable_post_process)glm_rotate_y(&view[0] ,time * 1.5, &view[0]);

    view[3][0] = 0.0f;
    view[3][1] = 0.0f;
    view[3][2] = 0.0f;
    view[3][3] = 1.0f; 

    aspect_ratio = (float)window_width / (float)window_height;
    glm_perspective(glm_rad(fov), aspect_ratio, 0.1f, 100.0f, projection);
    set_matrix(skybox_shader.shader_ID, "view", view);
    set_matrix(skybox_shader.shader_ID, "projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void run_post_processing(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window_width, window_height);  // Reset the viewport to the window size
        glDisable(GL_DEPTH_TEST); // Disable depth test so screen-space quad isn't discarded due to depth test.
        
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white
        glClear(GL_COLOR_BUFFER_BIT);

        use_shader(post_process_shader.shader_ID);
        
        set_int(post_process_shader.shader_ID, "screenWidth",  (int)window_width);
        set_int(post_process_shader.shader_ID, "screenHeight", (int)window_height);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // Use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
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