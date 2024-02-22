#version 330 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;
uniform float time;

void main()
{
   // TexCoord *= -1;
    FragColor = mix(texture(ourTexture, TexCoord + sin(time)), texture(texture2, vec2(TexCoord.x*5, TexCoord.y * 5)), 1);
}