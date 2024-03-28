#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec4 ourColor;
in vec3 Normal;
uniform sampler2D ourTexture;
uniform sampler2D texture2;
uniform float time;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    float ambientStrength = 0.08;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse =   diff * lightColor;
   
    // specular
    float specularStrength = 0.75;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.20), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (specular + ambient + diffuse ) * objectColor;
    FragColor = vec4(result, 1.0);
}