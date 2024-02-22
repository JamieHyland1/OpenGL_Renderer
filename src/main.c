#define GLEW_STATIC
#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <C:\SDL2\include\SDL.h>
#include <C:\SDL2\include\SDL_opengl.h>
#include "math.h"
#include "shader.h"
#include "texture.h"
//#include "../include/glad/glad.h"

static int window_width = 640;
static int window_height = 480;

unsigned int VBO;
unsigned int VAO;

char* filenames[2];
unsigned int EBO;

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

float time = 0;

shader_t shader;
texture_t tex;
texture_t tex2;
 float vertices[] = {
       // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

float texCoords[] = {
    0.5f, 1.0f,   // top-center corner
    1.0f, 0.0f,   // lower-right corner
    0.0f, 0.0f   // lower-left corner  
};

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0; 
#define FPS 60
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

    // int full_screen_width = displayMode.w;
    // int full_screen_height = displayMode.h;

    // window_width = full_screen_width/3;
    // window_height = full_screen_height/2;
    filenames[0] = "./shaders/vertex_shader.glsl";
    filenames[1] = "./shaders/fragment_shader.glsl";

    stbi_set_flip_vertically_on_load(true);

    //TODO create SDL window
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

    //TODO create SDL Renderer
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


    return true; 
}

int init_openGL(){
    if(init_shader(&shader, filenames)){
        printf("initialized shaders\n");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        
        
        tex  = init_texture("./textures/lffy.png");
        tex2 = init_texture("./textures/gear5.jpg");
        glBindVertexArray(0); 
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
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                }
            }
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
    
    // printf("time: %f\n",time);
   


    previous_frame_time = SDL_GetTicks();
}
///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, tex2.id);
    
    use_shader(shader.shader_ID);
    set_int(shader.shader_ID,"ourTexture",0);
    set_int(shader.shader_ID,"texture2",1);
    set_float(shader.shader_ID,"time",time);


    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void free_resources(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]){
    setup();
    is_running = init_openGL();
    glViewport(0, 0, 640, 480);
    while (is_running) {
        process_input();
        update();
        render();
    }

    free_resources();

    return 0;
}




