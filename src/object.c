#include "../include/headers/core.h"
#include <SDL2/SDL_thread.h>

object_t objects[MAX_NUM_OBJECTS_PER_SCENE] = {0};
int current_num_objects = 0;

bool init_object(object_t *obj, const char *model_path, const char *vert_path, const char *frag_path){
  obj->model = malloc(sizeof(model_t));
  if(!obj->model){
    printf("error allocating memory for model_t\n");
    return false;
  }
  *obj->model =(model_t){0};
  bool is_load_model = load_model(obj->model, model_path);
  if(!is_load_model){
    printf("Error loading model");
    return false;
  }
  obj->shader = malloc(sizeof(shader_t));
  if (!obj->shader) {
    fprintf(stderr, "Error allocating shader\n");
    free(obj->model);
    return false;
  }
  *obj->shader = (shader_t){0};
  bool load_shader = init_shader(obj->shader, vert_path, frag_path);
  if(!load_shader){
    printf("error initializing shaders");
    return false;
  }
  if(current_num_objects < MAX_NUM_OBJECTS_PER_SCENE)current_num_objects++;else return false;
  return true;
}



void draw_object(const object_t *object) {
    use_shader(object->shader);

    set_matrix(object->shader->shader_ID, "model",      object->transform);
    set_matrix(object->shader->shader_ID, "view",       view);
    set_matrix(object->shader->shader_ID, "projection", projection);

    draw_model(object->model, object->shader);
}
