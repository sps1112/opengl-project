#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emmision;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool check;

void main()
{
    // Setup values
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f/(light.constant+(light.linear * distance)+(light.quadratic * distance * distance));
    
    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

    // Diffused Lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse,TexCoords)));

    // Specular Lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * (vec3(texture(material.specular,TexCoords))));
    
    // Emmsion Lighting
    vec3 emmision=vec3(0.0f);
    if(texture(material.specular,TexCoords).r==0.0)
    {
        emmision = vec3(texture(material.emmision,TexCoords));
    }

    // Check for attenuation(default true)
    if(!check)
    {
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    // Resultant Lighting
    vec3 result = (ambient + diffuse + specular + emmision);
    FragColor=vec4(result, 1.0f);
}