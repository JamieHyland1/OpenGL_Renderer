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
#include "material.h"


//TODO: clean up all these variables into a display file or some sort of file that handles the settings of the renderer
static int window_width = 800;
static int window_height = 600;
unsigned int VBO;
unsigned int cubeVAO, lightVAO;
vec3 lightPos = {1.2f, 10.0f, 2.0f};
vec3 lightScale = {0.42f,0.42f,0.42f};
char* obj_shaders[2];
char* light_shaders[2];
unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
float time = 0;
float fov = -45.0f;
mat4 model,view,projection;
shader_t obj_shader, light_shader;
texture_t tex;
texture_t tex2;
vec3 axis = {0.5f,1.0f,0.0f};

vec3 cameraPos = {0.0f,-10.0f,13.0f};
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

material_t mat;
///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and renderer loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
static float delta_time = 0; 
#define FPS 144
#define FRAME_TARGET_TIME  (1000 / FPS)
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

    printf("material shininess %f" , mat.specular[0]);
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
        
        tex  = init_texture("./textures/container.jpg");
        tex2 = init_texture("./textures/jolly_roger.png");

        return 1;
    }else{
        printf("error initialising shaders\n");
    }
    return false;
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
            case SDL_MOUSEMOTION:
                process_mouse_move((float)event.motion.xrel,(float)event.motion.yrel,1);
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
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    
    // lightPos[0] += 5 * cos(time) * delta_time;
    // lightPos[2] += -5 * sin(time) * delta_time;


    float angle = fmodf(time,360.0f);
    float p_angle = fov;
    glm_make_rad(&angle);
    glm_make_rad(&p_angle);
    glm_rotate(&model[0], angle, &axis[0]);
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
    glm_rotate_y(&model[0], time, &model[0]);
    
    set_matrix(obj_shader.shader_ID,"model", model);
    set_matrix(obj_shader.shader_ID,"view", view);
    set_matrix(obj_shader.shader_ID,"projection", projection);

    get_camera_position(&cameraPos);
    //printf("%f,%f,%f\n", cameraPos[0],cameraPos[1],cameraPos[2]);
    set_float(obj_shader.shader_ID,"time",time);

    set_vec3(obj_shader.shader_ID, "lightColor", light);
    set_vec3(obj_shader.shader_ID, "lightPos", lightPos);
    set_vec3(obj_shader.shader_ID, "viewPos", cameraPos);
    //set_vec3(obj_shader.shader_ID, "material.ambient", mat.ambient);
    // set_vec3(obj_shader.shader_ID, "material.diffuse", mat.diffuse);
    set_vec3(obj_shader.shader_ID, "material.specular", mat.specular);
    set_float(obj_shader.shader_ID, "material.shininess", mat.shininess);
    set_int(obj_shader.shader_ID,"material.diffuse", 0);
    

    set_vec3(obj_shader.shader_ID, "light.ambient", (vec3){0.2f, 0.2f, 0.2f});
    set_vec3(obj_shader.shader_ID, "light.diffuse", (vec3){0.5f, 0.5f, 0.5f});
    set_vec3(obj_shader.shader_ID, "light.specular",(vec3){1.0f, 1.0f, 1.0f});
   
    glBindVertexArray(cubeVAO); 
    glDrawArrays(GL_TRIANGLES, 0, 36);

    use_shader(light_shader.shader_ID);

    glm_mat4_identity(model);
    set_matrix(light_shader.shader_ID,"view", view);
    set_matrix(light_shader.shader_ID,"projection", projection);
    glm_translate(&model[0], &lightPos[0]);
    glm_scale(&model[0], &lightScale[0]);
    set_matrix(light_shader.shader_ID,"model", model);
    set_vec3(light_shader.shader_ID, "lightColor", light);
   
    glBindVertexArray(lightVAO); 
    glDrawArrays(GL_TRIANGLES, 0, 36);
    


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
        process_input();
        update();
        render();
    }

    free_resources();

    return 0;
}




