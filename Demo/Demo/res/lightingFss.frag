#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

struct Light//light color
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material //material color
{
    //vec3 ambient;
    //vec3 diffuse;
	sampler2D diffuse;
    vec3 specular;
    float shininess;
};
uniform vec3 viewPos;
uniform Light light;
uniform Material material;
void main()
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	vec3 norm = normalize(Normal);//normalize normal
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));//diffuse relection

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);//specular relection
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);//result
}