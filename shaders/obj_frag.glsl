#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct Directional_Light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Point_Light{
    vec3 color;
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct Spot_Light{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Directional_Light light;
uniform Point_Light pLight;
uniform Spot_Light sLight;

uniform float time;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

vec3 directionalLight(){
        // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
    return  ambient + diffuse + specular ;


}

vec3 pointLight(){
       // ambient
    vec3 ambient = pLight.ambient * vec3(texture(material.diffuse, TexCoords));
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pLight.specular * spec * vec3(texture(material.specular, TexCoords));
    
    float distance    = length(pLight.position - FragPos);
    float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));  

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;   

    vec3 result =  (ambient + diffuse + specular) * pLight.color;

    return result;
}

vec3 spotLight(){
    vec3 lightDir = normalize(sLight.position - FragPos);
    float theta = dot(lightDir, normalize(-sLight.direction));
    float epsilon   = sLight.cutOff - sLight.outerCutOff;
    float intensity = clamp((theta - sLight.outerCutOff) / epsilon, 0.0, 1.0);   
    vec4 result;
        vec3 ambient = sLight.ambient * vec3(texture(material.diffuse, TexCoords));
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = sLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = sLight.specular * spec * vec3(texture(material.diffuse, TexCoords));
        
        // attenuation
        float distance    = length(sLight.position - FragPos);
        float attenuation = 1.0 / (sLight.constant + sLight.linear * distance + sLight.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
        diffuse   *= attenuation;
        specular  *= attenuation;   

        diffuse *= intensity;
        diffuse *= intensity;
            
        result = vec4(vec3(ambient + diffuse + specular),1.0f);//vec4(ambient + diffuse + specular,1.0);
    
    return vec3(result);
}

void main()
{
    vec3 result = vec3(0.0);
    result += directionalLight();
    result += pointLight();
    result += spotLight();


    FragColor = vec4(result, 1.0);
}