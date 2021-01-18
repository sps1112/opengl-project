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
    vec3 direction;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 result = vec3(0.0f);
    vec3 lightDir = normalize(light.position - FragPos);
    
    // cos value
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutoff- light.outerCutoff;
    float intensity = clamp(((theta-light.outerCutoff)/epsilon),0.0f,1.0f);

     // Diffused Lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse,TexCoords)));

    // Specular Lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * (vec3(texture(material.specular,TexCoords))));
    
    // Check for hard edge(smooth edge default)
    if(check)
    {
        if(theta>light.cutoff)
        {
             result = (diffuse + specular);
        }
    }
    else
    {
         result = (diffuse + specular)*intensity;
    }

    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

    // Emmision lighting
    vec3 emmision=vec3(0.0f);
    if(texture(material.specular,TexCoords).r==0.0)
    {
        emmision = vec3(texture(material.emmision,TexCoords));
    }

    // Resultant lighting
    result += ambient+ emmision;
    FragColor=vec4(result, 1.0f);
}