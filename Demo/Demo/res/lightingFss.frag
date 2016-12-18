#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

struct Light//Base Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirLight//Directional Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight//Point Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
struct Material //material color
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
    float shininess;
};
uniform vec3 viewPos;
uniform Light light;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform Material material;
void main()
{

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	vec3 norm = normalize(Normal);//normalize normal
	vec3 lightDir = normalize(light.position - FragPos);
	//vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));//diffuse relection

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));//specular relection

	//vec3 emission = vec3(texture(material.emission, TexCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;//attenuation

	color = vec4(ambient + diffuse + specular, 1.0f);//result
}