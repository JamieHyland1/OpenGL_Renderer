#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Read a shader file into a newly-allocated, null-terminated buffer.
// Caller must free() the result.
///////////////////////////////////////////////////////////////////////////////
GLchar* get_shader_source(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "ERROR::SHADER::Could not open file: %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    GLchar* buffer = malloc((size_t)size + 1);
    if (!buffer) {
        fprintf(stderr, "ERROR::SHADER::malloc failed for %s\n", filename);
        fclose(fp);
        return NULL;
    }

    size_t read = fread(buffer, 1, (size_t)size, fp);
    buffer[read] = '\0';
    fclose(fp);
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////
// Compile one shader stage. Returns the shader object ID, or 0 on failure.
///////////////////////////////////////////////////////////////////////////////
static GLuint compile_stage(const GLchar* source, GLenum gl_type, const char* type_name) {
    GLuint id = glCreateShader(gl_type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n", type_name, info_log);
        glDeleteShader(id);
        return 0;
    }
    return id;
}

///////////////////////////////////////////////////////////////////////////////
// Load, compile, and link both shader stages into a program in one call.
///////////////////////////////////////////////////////////////////////////////
bool init_shader(shader_t* shader, const char* vert_path, const char* frag_path) {
    GLchar* vert_source = get_shader_source(vert_path);
    GLchar* frag_source = get_shader_source(frag_path);

    if (!vert_source || !frag_source) {
        free(vert_source);
        free(frag_source);
        return false;
    }

    GLuint vert_id = compile_stage(vert_source, GL_VERTEX_SHADER, "VERTEX");
    if (vert_id == 0) {
        free(vert_source);
        free(frag_source);
        return false;
    }

    GLuint frag_id = compile_stage(frag_source, GL_FRAGMENT_SHADER, "FRAGMENT");
    if (frag_id == 0) {
        glDeleteShader(vert_id);
        free(vert_source);
        free(frag_source);
        return false;
    }

    // Link
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_id);
    glAttachShader(program, frag_id);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    // Shader objects are no longer needed once linked (or on failure)
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);

    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
        glDeleteProgram(program);
        free(vert_source);
        free(frag_source);
        return false;
    }

    // Success - store everything in the struct
    shader->shader_ID       = program;
    shader->vertex_source   = vert_source;
    shader->fragment_source = frag_source;
    shader->path_to_vert    = vert_path;
    shader->path_to_frag    = frag_path;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Rebuild from source files. Keeps the old program if reload fails.
///////////////////////////////////////////////////////////////////////////////
bool reload_shader(shader_t* shader) {
    shader_t tmp = {0};

    if (!init_shader(&tmp, shader->path_to_vert, shader->path_to_frag)) {
        return false;   // old shader untouched, keep rendering with it
    }

    // Success - discard the old program + sources, swap in the new
    glDeleteProgram(shader->shader_ID);
    free(shader->vertex_source);
    free(shader->fragment_source);

    *shader = tmp;

    printf("Reloaded shader: %s / %s\n", shader->path_to_vert, shader->path_to_frag);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Point a broken shader at the prebuilt error program.
///////////////////////////////////////////////////////////////////////////////
void load_error_shader(shader_t* shader, shader_t* err) {
    shader->shader_ID = err->shader_ID;
}

///////////////////////////////////////////////////////////////////////////////
// Free GPU + heap resources.
///////////////////////////////////////////////////////////////////////////////
void free_shader(shader_t* shader) {
    if (shader->shader_ID) glDeleteProgram(shader->shader_ID);
    free(shader->vertex_source);
    free(shader->fragment_source);

    shader->shader_ID       = 0;
    shader->vertex_source   = NULL;
    shader->fragment_source = NULL;
}

void use_shader(const shader_t* shader) {
    glUseProgram(shader->shader_ID);
}

///////////////////////////////////////////////////////////////////////////////
// Uniform setters
///////////////////////////////////////////////////////////////////////////////
void set_bool(GLuint id, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void set_int(GLuint id, const char* name, int value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void set_float(GLuint id, const char* name, float value) {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void set_vec3(GLuint id, const char* name, vec3 value) {
    glUniform3f(glGetUniformLocation(id, name), value[0], value[1], value[2]);
}

void set_matrix(GLuint id, const char* name, mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, (const float*)mat);
}
