#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D specular;
    float     shininess;
}; 
uniform float test;
out vec4 FragColor;
void main()
{
    float x = 0;
    x += time;
    vec4 col = texture(texture_diffuse1, TexCoord);
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}