#version 410 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vertNormal;
in vec3 fpos;
in vec3 viewPos;
in vec3 viewDir;

out vec4 fragColor;

uniform Material material;
uniform Light light;


void main(void) {

   vec3 ambient = light.ambient * material.ambient;
  	
    // Diffuse 
   vec3 norm = normalize(vertNormal);
   vec3 lightDir = normalize(light.position - fpos);
   float diff = max(dot(norm, lightDir), 0.0f);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);
  
   //Specular
   vec3 viewDir = normalize(viewPos - fpos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient*(0.2f) + diffuse + specular;
   // color = vec4(result, 1.0f);

	fragColor = vec4(result, 1.0f);
    		
}