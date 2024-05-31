#version 330 core

out vec4 FragColor;

in vec4 ourColor;

uniform sampler2D ourTexture;
uniform sampler2D texture2;
uniform float time;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
}