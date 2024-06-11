#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoord;
out vec3 aPos2;
uniform float time;
float x = 0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
    aPos2 = aPos;
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    TexCoord = aTexCoords;
}