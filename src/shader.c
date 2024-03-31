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

bool init_shader(shader_t* shader, char* shader_filenames[2]){
    shader->vertex_source   = get_shader_source(shader_filenames[0]);
    shader->fragment_source = get_shader_source(shader_filenames[1]);
    
    int success;
    char infoLog[512];


    // //if file reading was successful link and compile vertex and fragment shaders
    if(shader->fragment_source != NULL && shader->vertex_source != NULL){

        shader->vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader->vertex_shader_ID,1,&shader->vertex_source,NULL);
        glCompileShader(shader->vertex_shader_ID);

        glGetShaderiv(shader->vertex_shader_ID,GL_COMPILE_STATUS,&success);

        if (!success){
            glGetShaderInfoLog(shader->vertex_shader_ID, 512, NULL, infoLog);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", infoLog);
            return false;
        }

        shader->fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader->fragment_shader_ID,1,&shader->fragment_source,NULL);
        glCompileShader(shader->fragment_shader_ID);

        glGetShaderiv(shader->fragment_shader_ID,GL_COMPILE_STATUS,&success);
        if (!success){
            glGetShaderInfoLog(shader->fragment_shader_ID, 512, NULL, infoLog);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", infoLog);
            return false;
        }


        shader->shader_ID = glCreateProgram();

        glAttachShader(shader->shader_ID,shader->vertex_shader_ID);
        glAttachShader(shader->shader_ID,shader->fragment_shader_ID);

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
    return false;
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
