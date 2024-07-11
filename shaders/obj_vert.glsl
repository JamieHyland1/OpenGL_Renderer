#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 aPos2;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int quantize;
void main(){
    aPos2 = aPos;
    vec4 viewPosition = view * model * vec4(aPos, 1.0);
    
    if(quantize > 1)
        viewPosition.xyz = floor(viewPosition.xyz * quantize) / quantize;
    

    // Transform the quantized position to clip space
    gl_Position = projection * viewPosition;

    TexCoord = aTexCoords;
}