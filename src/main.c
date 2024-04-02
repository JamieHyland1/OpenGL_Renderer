#define GLEW_STATIC
#include "../include/GL/glew.h"
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "../include/cglm/cglm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "windows.h"
#include "material.h"
#include "fileapi.h"
#include "synchapi.h"
#include "pthread.h"


//TODO: clean up all these variables into a display file or some sort of file that handles the settings of the renderer
static int window_width = 800;
static int window_height = 600;
bool mouse_button_down = false;
unsigned int VBO;
unsigned int cubeVAO, lightVAO;
DWORD shaderStatus;
vec3 lightPos[] = { {-0.2f, 2.0f, -2.0f},
                    {-1.2f, 0.0f, -1.0f},
                    {0.2f, -2.0f, 2.0f},};

vec3 lightCol[] = { {1.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f},
                    {0.0f, 0.0f, 1.0f}};
vec3 lightDir = {-0.2f, 2.0f, -2.0f};
vec3 lightScale = {0.42f,0.42f,0.42f};
vec3 spotLightPos = {0.0f,0.0f,0.0f};
vec3 spotLightDir = {0.0f,0.0f,0.0f};
float spotLightCutOffInner = 12.0f;
float spotLightCutOffOuter = 15.0f;


char* obj_shaders[2];
char* light_shaders[2];
unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
float time = 0;
float fov = 45.0f;
mat4 model,view,projection;
shader_t obj_shader, light_shader;
texture_t tex;
texture_t tex2;
vec3 axis = {1.0f, 0.3f, 0.5f};
HANDLE dwChangeHandles, files; 
vec3 cameraPos = {0.0f, 0.0f, 5.0f};
vec3 up = {0.0f,1.0f,0.0f};

float vertices[] = {
     // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
unsigned int indices[] = {
   -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f,0.0f,
     0.0f,  0.5f, 0.5f,  0.0f, 0.0f,1.0f
    };
vec3 cubePositions[] = {
    { 0.0f,  0.0f,  -3.5f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
};
material_t mat;
///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and renderer loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
float poll_timer = 0;
static float delta_time = 0; 
#define FPS 144
#define FRAME_TARGET_TIME  (1000 / FPS)
#define SHADER_POLL_TIME  (2500/1000)
static SDL_Window* window = NULL;
SDL_GLContext context = NULL;
static SDL_Renderer* renderer = NULL;
///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
int setup(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    fprintf(stderr, "Error initializing SDL window");
    return false;
    }
    pthread_t thread1;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode displayMode;

    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    set_material_ambient(&mat, (vec3){1.0f, 0.5f, 0.31f});
    set_material_diffuse(&mat, (vec3){1.0f, 0.5f, 0.31f});
    set_material_specular(&mat,(vec3){0.5f, 0.5f, 0.5f});
    set_material_shininess(&mat, 32.0f);

   
    // int full_screen_width = displayMode.w;
    // int full_screen_height = displayMode.h;
    // window_width = full_screen_width;
    // window_height = full_screen_height;

    obj_shaders[0] = "./shaders/obj_vertex.glsl";
    obj_shaders[1] = "./shaders/obj_frag.glsl";
    light_shaders[0] = "./shaders/light_vertex.glsl";
    light_shaders[1] = "./shaders/light_frag.glsl";

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
// Init function for openGL to set its various parameters
///////////////////////////////////////////////////////////////////////////////
int init_openGL(){
    if(init_shader(&obj_shader, obj_shaders) && init_shader(&light_shader,light_shaders)){
        printf("initialized shaders\n");
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute


        glEnable(GL_DEPTH_TEST); 
        
        stbi_set_flip_vertically_on_load(true);
        
        tex  = init_texture("./textures/container.png");
        tex2 = init_texture("./textures/container2_specular.png");

        return 1;
    }else{
        printf("error initialising shaders\n");
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// Poll shader directory for changes to shaders and reload if necessary
///////////////////////////////////////////////////////////////////////////////
void process_shader_changes(void){
        dwChangeHandles =  FindFirstChangeNotificationA("./shaders/",false,FILE_NOTIFY_CHANGE_LAST_WRITE);
        shaderStatus = WaitForSingleObject(dwChangeHandles,200);
        switch(shaderStatus){
            case WAIT_OBJECT_0:
                printf("Wait object 0\n");
                WIN32_FIND_DATA FindFileData;
                files = FindFirstFileA("./shaders/*.glsl",&FindFileData);
                if(files == INVALID_HANDLE_VALUE){
                    printf("couldnt find files :( %ld\n",GetLastError());
                }else{
                    while (FindNextFileA(files, &FindFileData) != 0){
                        printf("file changed: %s\n",FindFileData.cFileName);
                    }
                    FindClose(files);
                }
            break;
            case WAIT_ABANDONED:
               // printf("Wait abandoned\n");
            break;
            case WAIT_TIMEOUT:
               // printf("Wait timeout\n");
            break;
            case WAIT_FAILED:
                //printf("wait failed\n");
            break;
    }

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
    spotLightCutOffInner = 12.5f;
    spotLightCutOffOuter = 17.5f;
    float p_angle = fov;
    glm_make_rad(&p_angle);

    glm_make_rad(&spotLightCutOffInner);
    spotLightCutOffInner = cosf(spotLightCutOffInner);

    glm_make_rad(&spotLightCutOffOuter);
    spotLightCutOffOuter = cosf(spotLightCutOffOuter);
    get_camera_position(&spotLightPos);
    get_camera_direction(&spotLightDir);

    
    lightPos[2][1] = 2.5 * sin(time);
    lightPos[2][2] = 2.5 * cos(time*2);
    
    camera_look_at(&view);
    glm_perspective(p_angle,(float)(window_width/window_height),0.1f,100.0f,projection);
    
    previous_frame_time = SDL_GetTicks();
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.0f,0.0f,0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, tex2.id);
    
    use_shader(obj_shader.shader_ID);
    vec3 light = {1.0f,1.0f,1.0f};
    

    set_matrix(obj_shader.shader_ID,"view", view);
    set_matrix(obj_shader.shader_ID,"projection", projection);



    get_camera_position(&cameraPos);
    set_float(obj_shader.shader_ID,"time",time);

    set_vec3(obj_shader.shader_ID, "lightColor", light);
    set_vec3(obj_shader.shader_ID, "light.direction", (vec3){-0.2f, -1.0f, -0.3f});
    set_vec3(obj_shader.shader_ID, "viewPos", cameraPos);

    set_float(obj_shader.shader_ID, "material.shininess", mat.shininess);
    set_int(obj_shader.shader_ID,"material.diffuse", 0);
    set_int(obj_shader.shader_ID,"material.specular", 1);
    
    //For directional Lights
    set_vec3(obj_shader.shader_ID, "light.ambient", (vec3){0.02f, 0.02f, 0.02f});
    set_vec3(obj_shader.shader_ID, "light.diffuse", (vec3){0.5f, 0.5f, 0.5f});
    set_vec3(obj_shader.shader_ID, "light.specular",(vec3){1.0f, 1.0f, 1.0f});
    set_vec3(obj_shader.shader_ID, "light.direction", lightDir);

    //For point Lights
    
        set_vec3(obj_shader.shader_ID, "pointLights[0].color", lightCol[0]);
        set_vec3(obj_shader.shader_ID, "pointLights[0].ambient", (vec3){0.2f, 0.82f, 0.2f});
        set_vec3(obj_shader.shader_ID, "pointLights[0].diffuse", (vec3){0.5f, 0.5f, 0.5f});
        set_vec3(obj_shader.shader_ID, "pointLights[0].specular",(vec3){1.0f, 0.0f, 1.0f});
        set_vec3(obj_shader.shader_ID, "pointLights[0].position", lightPos[0]);
        set_float(obj_shader.shader_ID, "pointLights[0].constant", 1.0f);
        set_float(obj_shader.shader_ID, "pointLights[0].linear", 0.09f);
        set_float(obj_shader.shader_ID, "pointLights[0].quadratic", 0.032f);
    
        set_vec3(obj_shader.shader_ID, "pointLights[1].color", lightCol[1]);
        set_vec3(obj_shader.shader_ID, "pointLights[1].ambient", (vec3){0.25f, 0.02f, 0.02f});
        set_vec3(obj_shader.shader_ID, "pointLights[1].diffuse", (vec3){0.05f, 0.5f, 0.5f});
        set_vec3(obj_shader.shader_ID, "pointLights[1].specular",(vec3){1.0f, 1.0f, 1.0f});
        set_vec3(obj_shader.shader_ID, "pointLights[1].position", lightPos[1]);
        set_float(obj_shader.shader_ID, "pointLights[1].constant", 1.0f);
        set_float(obj_shader.shader_ID, "pointLights[1].linear", 0.09f);
        set_float(obj_shader.shader_ID, "pointLights[1].quadratic", 0.032f);

        set_vec3(obj_shader.shader_ID, "pointLights[2].color", lightCol[2]);
        set_vec3(obj_shader.shader_ID, "pointLights[2].ambient", (vec3){0.52f, 0.52f, 0.52f});
        set_vec3(obj_shader.shader_ID, "pointLights[2].diffuse", (vec3){0.5f, 0.25f, 0.5f});
        set_vec3(obj_shader.shader_ID, "pointLights[2].specular",(vec3){1.0f, 1.0f, 1.0f});
        set_vec3(obj_shader.shader_ID, "pointLights[2].position", lightPos[2]);
        set_float(obj_shader.shader_ID, "pointLights[2].constant", 1.0f);
        set_float(obj_shader.shader_ID, "pointLights[2].linear", 0.9f);
        set_float(obj_shader.shader_ID, "pointLights[2].quadratic", 0.032f);
    

    //For spot Light
    set_vec3(obj_shader.shader_ID, "sLight.ambient", (vec3){0.1f, 0.1f, 0.1f});
    set_vec3(obj_shader.shader_ID, "sLight.diffuse", (vec3){0.8f, 0.8f, 0.8f});
    set_vec3(obj_shader.shader_ID, "sLight.specular",(vec3){1.0f, 1.0f, 1.0f});
    set_float(obj_shader.shader_ID, "sLight.constant", 1.0f);
    set_float(obj_shader.shader_ID, "sLight.linear", 0.09f);
    set_float(obj_shader.shader_ID, "sLight.quadratic", 0.032f);
    set_vec3(obj_shader.shader_ID, "sLight.position", spotLightPos);
    set_vec3(obj_shader.shader_ID, "sLight.direction", spotLightDir);
    set_float(obj_shader.shader_ID, "sLight.cutOff", spotLightCutOffInner);
    set_float(obj_shader.shader_ID, "sLight.outerCutOff", spotLightCutOffOuter);

    glBindVertexArray(cubeVAO); 

    for(int i = 0; i < 10; i ++){
        glm_mat4_identity(&model[0]);
        glm_translate(&model[0], &cubePositions[i][0]);
        float angle = i * 20.0f;
        glm_rotate(&model[0], angle, &axis[0]);
        set_matrix(obj_shader.shader_ID,"model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    use_shader(light_shader.shader_ID);
    set_matrix(light_shader.shader_ID,"view", view);
    set_matrix(light_shader.shader_ID,"projection", projection);
    for(int i = 0; i < 3; i ++){
        glm_mat4_identity(model);
        glm_translate(&model[0], &lightPos[i][0]);
        glm_scale(&model[0], &lightScale[0]);
        set_matrix(light_shader.shader_ID,"model", model);
        set_vec3(light_shader.shader_ID, "lightColor", lightCol[i]);
        
        glBindVertexArray(lightVAO); 
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    


    SDL_GL_SwapWindow(window);
}
///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

}
///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]){
    setup();
    is_running = init_openGL();
    glViewport(0, 0, window_width, window_height);
    while (is_running) {
        process_shader_changes();
        process_input();
        update();
        render();
    }

    free_resources();

    return 0;
}




