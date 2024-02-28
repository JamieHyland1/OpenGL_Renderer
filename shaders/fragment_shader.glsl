#version 330 core

out vec4 FragColor;
  

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;
uniform float time;

void main()
{
   // TexCoord *= -1;
    FragColor = texture(ourTexture, TexCoord);//mix(texture(ourTexture, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.5);
}