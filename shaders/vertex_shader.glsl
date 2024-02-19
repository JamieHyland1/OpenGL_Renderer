#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;


out vec4 ourColor;

void main(){
   gl_Position = vec4(aPos.x, aPos.y * -1.0, aPos.z, 1.0);
   ourColor = vec4(aCol,1.0);
}