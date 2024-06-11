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

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    vec4 c = texture(material.texture_diffuse1,TexCoord);
    //FragColor = c;
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
}