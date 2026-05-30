#include "../include/headers/core.h"
#include "../include/headers/light.h"
#include "../include/headers/shader.h"
#include <GL/glew.h>

light_t lights_in_scene[MAX_NUMBER_OF_LIGHTS_PER_SCENE];
static int current_num_lights = 0;

bool make_point_light(vec3 position, vec3 color, float intensity, float range){

  if(current_num_lights < MAX_NUMBER_OF_LIGHTS_PER_SCENE){

      lights_in_scene[current_num_lights].position[0] = position[0];
      lights_in_scene[current_num_lights].position[1] = position[1];
      lights_in_scene[current_num_lights].position[2] = position[2];

      lights_in_scene[current_num_lights].color[0] = color[0];
      lights_in_scene[current_num_lights].color[1] = color[1];
      lights_in_scene[current_num_lights].color[2] = color[2];

      lights_in_scene[current_num_lights].intensity = intensity;
      lights_in_scene[current_num_lights].range = range;
      current_num_lights ++;
      return true;

  }
  return false;
}

bool make_directional_light(vec3 position, vec3 color, float intensity){
  light_t light = (light_t){0};
  light.position[0] = position[0];
  light.position[1] = position[1];
  light.position[2] = position[2];

  light.color[0] = color[0];
  light.color[1] = color[1];
  light.color[2] = color[2];

  light.intensity = intensity;

  return false;
}

bool make_spot_light(vec3 position, vec3 direction, vec3 color, float intensity, float inner_cone, float outer_cone){
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
  
  return false;
}


int get_num_lights(){
  return current_num_lights;
}

void light_apply_to_shader(GLuint shader_id, int light_index){
    set_vec3(shader_id,  "light_position",      lights_in_scene[light_index].position);
    set_vec3(shader_id,  "light_color",         lights_in_scene[light_index].color);
    set_vec3(shader_id,  "light_direction",     lights_in_scene[light_index].direction);
    set_float(shader_id, "light_intensity",     lights_in_scene[light_index].intensity);
    set_float(shader_id, "light_range",         lights_in_scene[light_index].range);
    set_float(shader_id, "light_inner_cutoff",  lights_in_scene[light_index].inner_cutoff);
    set_float(shader_id, "light_outer_cutoff",  lights_in_scene[light_index].outer_cutoff);
}
