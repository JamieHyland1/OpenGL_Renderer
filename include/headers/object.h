#ifndef object_h
#define object_h

#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <stdbool.h>

#include "shader.h"
#include "model.h"

typedef struct {
    model_t*  model;
    shader_t* shader;
    vec3      position;
    vec3      rotation;
    vec3      scale;
    mat4     transform;
} object_t;

#define MAX_NUM_OBJECTS_PER_SCENE 128
extern int current_num_objects;
extern object_t objects[MAX_NUM_OBJECTS_PER_SCENE];
extern object_t object;

bool init_object(object_t* obj,
                 const char* model_path,
                 const char* vert_path,
                 const char* frag_path);

void free_object(object_t* obj);
void draw_object(const object_t* obj);

#endif
