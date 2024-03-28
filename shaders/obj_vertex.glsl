#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out vec4 ourColor;
out vec3 Normal;
out vec3 FragPos;  
uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos,1.0f);//vec4((aPos.x + sin(time)),aPos.y + cos(time),aPos.z + sin(time), 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    ourColor = aColor;
    FragPos = vec3(model * vec4(aPos,1.0f));
}