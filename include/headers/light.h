#ifndef light_h
#define light_h
#include <stdbool.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

enum light_type{
  POINT_LIGHT,
  DIRECTIONAL_LIGHT,
  SPOT_LIGHT
};

typedef unsigned int GLuint;

typedef struct {
   enum light_type type;
    vec3 color;
    float intensity;
    
    vec3 position;
    float range;
    
    vec3 direction;
    
    float inner_cutoff;
    float outer_cutoff;
} light_t;

#define MAX_NUMBER_OF_LIGHTS_PER_SCENE 10
extern light_t lights_in_scene[MAX_NUMBER_OF_LIGHTS_PER_SCENE];

bool make_point_light(vec3 position, vec3 color, float intensity, float range);
bool make_directional_light(vec3 direction, vec3 color, float intensity);
bool make_spot_light(vec3 position, vec3 direction, vec3 color, 
                       float intensity, float inner_cone, float outer_cone);
int get_num_lights();
void light_apply_to_shader(GLuint shader_id, int light_index);

#endif
