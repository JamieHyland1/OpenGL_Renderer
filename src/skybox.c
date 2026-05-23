#include "skybox.h"
#include "stb/stb_image.h"
#include <stdio.h>
#include <stdlib.h>

static const float skybox_vertices[] = {
    -1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
};

skybox_t skybox = {0};
// faces order: +X, -X, +Y, -Y, +Z, -Z (right, left, top, bottom, front, back)
cubemap_t load_cubemap(const char* faces[6]) {
    cubemap_t cubemap = {0};

    glGenTextures(1, &cubemap.texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.texture_id);

    stbi_set_flip_vertically_on_load(false);  // cubemaps are NOT flipped

    for (int i = 0; i < 6; i++) {
        int w, h, ch;
        unsigned char* data = stbi_load(faces[i], &w, &h, &ch, 0);
        if (data) {
            GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
                         w, h, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            fprintf(stderr, "ERROR::CUBEMAP::Failed to load face: %s\n", faces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return cubemap;
}

bool init_skybox(skybox_t* sky, const char* faces[6],
                 const char* vert_path, const char* frag_path) {
    *sky = (skybox_t){0};

    sky->cubemap = load_cubemap(faces);          // composed - store the whole cubemap_t
    if (sky->cubemap.texture_id == 0) {
        fprintf(stderr, "ERROR::SKYBOX::Cubemap failed to load\n");
        return false;
    }

    glGenVertexArrays(1, &sky->VAO);
    glGenBuffers(1, &sky->VBO);

    glBindVertexArray(sky->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sky->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    if (!init_shader(&sky->shader, vert_path, frag_path)) {
        fprintf(stderr, "ERROR::SKYBOX::Shader failed to load\n");
        return false;
    }

    return true;
}

void draw_skybox(const skybox_t* sky, mat4 view, mat4 projection) {
    glDepthFunc(GL_LEQUAL);

    use_shader(&sky->shader);

    mat4 view_no_translation;
    glm_mat4_copy(view, view_no_translation);
    view_no_translation[3][0] = 0.0f;
    view_no_translation[3][1] = 0.0f;
    view_no_translation[3][2] = 0.0f;

    set_matrix(sky->shader.shader_ID, "view", view_no_translation);
    set_matrix(sky->shader.shader_ID, "projection", projection);

    glBindVertexArray(sky->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky->cubemap.texture_id);   // composed access
    set_int(sky->shader.shader_ID, "skybox", 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void free_skybox(skybox_t* sky) {
    if (sky->cubemap.texture_id) glDeleteTextures(1, &sky->cubemap.texture_id);
    if (sky->VAO)                glDeleteVertexArrays(1, &sky->VAO);
    if (sky->VBO)                glDeleteBuffers(1, &sky->VBO);
    free_shader(&sky->shader);

    *sky = (skybox_t){0};
}
