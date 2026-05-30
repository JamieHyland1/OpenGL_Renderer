#version 330 core

out vec4 FragColor;
uniform vec4  diffuse_color;

uniform vec3  light_position;
uniform vec3  light_color;
uniform vec3  light_direction;
uniform float light_intensity;
uniform float light_range;
uniform float light_inner_cutoff;
uniform float light_outer_cutoff;


void main()
{
    vec4 final_color = vec4(1,1,1,1);

    final_color.rgb *= light_color;
    final_color.rgb *= light_intensity;

    FragColor = final_color;
}
