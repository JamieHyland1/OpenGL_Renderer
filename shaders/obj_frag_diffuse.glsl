#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D specular;
    float     shininess;
}; 
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
uniform Material material;

void main()
{
    vec4 tex_color = texture(material.texture_diffuse1,TexCoord);
    // if(tex_color.a < 0.1){
    //     discard;
    // }
    FragColor = tex_color;
}