#include "../include/headers/core.h"
#include "../include/headers/light.h"
#include <GL/glew.h>


light_t make_point_light(vec3 position, vec3 color, float intensity, float range){
  light_t light = (light_t){0};
  light.position[0] = position[0];
  light.position[1] = position[1];
  light.position[2] = position[2];

  light.color[0] = color[0];
  light.color[1] = color[1];
  light.color[2] = color[2];

  light.intensity = intensity;
  light.range = range;

  return light;
}

light_t make_directional_light(vec3 position, vec3 color, float intensity){
  light_t light = (light_t){0};
  light.position[0] = position[0];
  light.position[1] = position[1];
  light.position[2] = position[2];

  light.color[0] = color[0];
  light.color[1] = color[1];
  light.color[2] = color[2];

  light.intensity = intensity;

  return light;
}

light_t make_spot_light(vec3 position, vec3 direction, vec3 color, float intensity, float inner_cone, float outer_cone){
  light_t light = (light_t){0};
  light.position[0] = position[0];
  light.position[1] = position[1];
  light.position[2] = position[2];

  light.color[0] = color[0];
  light.color[1] = color[1];
  light.color[2] = color[2];

  light.intensity = intensity;
  light.inner_cutoff = inner_cone;
  light.outer_cutoff = outer_cone;
  
  return light;
}

void light_apply_to_shader(const light_t *light, GLuint shader_id, int light_index){
    /* TODO add this and then apply to scene */
}
