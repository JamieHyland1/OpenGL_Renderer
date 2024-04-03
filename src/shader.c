#include "../include/GL/glew.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <C:\SDL2\include\SDL.h>
#include "shader.h"
#include "../include/cglm/cglm.h"

///////////////////////////////////
// This class handles shaders
// specifically initializing  them,  
// using them and passing various uniforms at runtime 
///////////////////////////////////
bool init_shader(shader_t* shader, char* filename, Shader_Type type){
    if(type == VERTEX){
        shader->vertex_source   = get_shader_source(filename);
        shader->path_to_vert = filename;
    }else if(type == FRAGMENT){
        shader->fragment_source = get_shader_source(filename);
        shader->path_to_frag = filename;
    }
    int success;
    char infoLog[512];


    //if file reading was successful link and compile vertex and fragment shaders
    if(type == VERTEX && shader->vertex_source != NULL){
        shader->vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader->vertex_shader_ID,1,&shader->vertex_source,NULL);
        glCompileShader(shader->vertex_shader_ID);

        glGetShaderiv(shader->vertex_shader_ID,GL_COMPILE_STATUS,&success);

        if (!success){
            glGetShaderInfoLog(shader->vertex_shader_ID, 512, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", infoLog);
            return false;
        }
    }

    if(type == FRAGMENT && shader->fragment_source != NULL){
        shader->fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader->fragment_shader_ID,1,&shader->fragment_source,NULL);
        glCompileShader(shader->fragment_shader_ID);

        glGetShaderiv(shader->fragment_shader_ID,GL_COMPILE_STATUS,&success);
        if (!success){
            glGetShaderInfoLog(shader->fragment_shader_ID, 512, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", infoLog);
            return false;
        }
    }
    shader->shader_ID = glCreateProgram();

    glAttachShader(shader->shader_ID,shader->vertex_shader_ID);
    glAttachShader(shader->shader_ID,shader->fragment_shader_ID);

   

    return true;
}
bool link_shader(shader_t* shader){
    int success;
    char infoLog[512];
    glLinkProgram(shader->shader_ID);

    glGetProgramiv(shader->shader_ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader->shader_ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED%s\n", infoLog);
        return false;
    }
    // delete shaders once linked successfully
    glDeleteShader(shader->vertex_shader_ID);
    glDeleteShader(shader->fragment_shader_ID);

    return true;
}

bool reload_shader(shader_t* shader){
    shader_t new_shader;
    Shader_Type frag = FRAGMENT;
    Shader_Type vert = VERTEX;
   // printf("yatta! %s\n", shader->fragment_source);
  //  printf("yatta! %s\n", shader->vertex_source);
    if(!init_shader(&new_shader, shader->path_to_frag,FRAGMENT))
        return false;
    if(!init_shader(&new_shader, shader->path_to_vert,VERTEX))
        return false;
    if(!link_shader(&new_shader))
        return false;
    
 //   printf("yatta!\n");
    glDeleteProgram(&shader->shader_ID);
    shader->shader_ID = new_shader.shader_ID;

}
void use_shader(int id){
    glUseProgram(id);
}

void set_bool  (int id, char* name, bool value){
    glUniform1i(glGetUniformLocation(id,name),value);
}

void set_int   (int id, char* name, int value){
    glUniform1i(glGetUniformLocation(id,name),value);
}

void set_float (int id, char* name, float value){
    glUniform1f(glGetUniformLocation(id,name),value);
}

void set_vec3 (int id, char* name, vec3 color){
    glUniform3f(glGetUniformLocation(id, name),color[0],color[1],color[2]);
}

void set_matrix(int id, char* name, mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(id, name),1,GL_FALSE,(float *)mat);
}

GLchar* get_shader_source(char* filename){
    GLchar* buffer = NULL;
    size_t size = 0;
    FILE *fp = fopen(filename,"rb");
    
    if(fp){
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        rewind(fp);
        buffer = malloc(( size + 1 ) * sizeof(buffer));
        fread(buffer,size,1,fp);
        buffer[size] = '\0';
        return buffer;
    }
    printf("something went wrong\n");
    return NULL;
}
