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

light_t make_point_light(vec3 position, vec3 color, float intensity, float range);
light_t make_directional_light(vec3 direction, vec3 color, float intensity);
light_t make_spot_light(vec3 position, vec3 direction, vec3 color, 
                       float intensity, float inner_cone, float outer_cone);
void light_apply_to_shader(const light_t* light, GLuint shader_id, int light_index);

#endif
