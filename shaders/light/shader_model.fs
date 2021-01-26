#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_emmision1;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight
{
    vec3 position;
    vec3 direction;
    
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 GetAmbient(vec3 ambient);
vec3 GetDiffuse(vec3 normal, vec3 lightDir, vec3 diffuse);
vec3 GetSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specular);

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;

void main()
{
    // Scene Lighting
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos- FragPos);
    vec3 result=vec3(0.0f);
    
    // 1:- Directional Light
    result+=CalculateDirLight(dirLight,norm,viewDir);
    
    // 2:- Point Lights
    for(int i=0;i<NR_POINT_LIGHTS;i++)
    {
        result+=CalculatePointLight(pointLights[i],norm,FragPos,viewDir);
    }

    // 3:- SpotLight
    result+=CalculateSpotLight(spotLight,norm,FragPos,viewDir);

    // Other effects
    vec3 emmision=vec3(0.0f);
    if(texture(material.texture_specular1,TexCoords).r==0.0)
    {
        emmision = vec3(texture(material.texture_emmision1,TexCoords));
    }
    
    // Resultant Lighting
    result += (emmision);
    FragColor=vec4(result, 1.0f); 
}

vec3 CalculateDirLight(DirLight light,vec3 normal, vec3 viewDir)
{
    // Setup Values
    vec3 lightDir=normalize(-light.direction);
    // Ambient Lighting
    vec3 ambient = GetAmbient(light.ambient);
    // Diffused lighting
    vec3 diffuse = GetDiffuse(normal, lightDir, light.diffuse);
    // Specular Lighting
    vec3 specular = GetSpecular(normal, lightDir, viewDir, light.specular);
    // Resultant Output
    vec3 final = (ambient + diffuse + specular);
    return final;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Setup values
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f/(light.constant+(light.linear * distance)+(light.quadratic * distance * distance));
    // Ambient Lighting
    vec3 ambient = GetAmbient(light.ambient);
    // Diffused Lighting
    vec3 diffuse = GetDiffuse(normal, lightDir, light.diffuse);
    // Specular Lighting
    vec3 specular = GetSpecular(normal, lightDir, viewDir, light.specular);
    // Resultant Light
    vec3 final = (ambient + diffuse + specular) * attenuation;
    return final;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Setup values
    vec3 lightDir = normalize(light.position - fragPos);
    // cos value
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutoff- light.outerCutoff;
    float intensity = clamp(((theta-light.outerCutoff)/epsilon),0.0f,1.0f);

    // Ambient Lighting
    vec3 ambient = GetAmbient(light.ambient);
    // Diffused Lighting
    vec3 diffuse = GetDiffuse(normal, lightDir, light.diffuse);
    // Specular Lighting
    vec3 specular = GetSpecular(normal, lightDir, viewDir, light.specular);

    // Resultant lighting
    vec3 final = (diffuse + specular)*intensity;
    final += ambient;
    return final;
}

vec3 GetAmbient(vec3 ambient)
{
    vec3 final = ambient * vec3(texture(material.texture_diffuse1,TexCoords));
    return final;
}

vec3 GetDiffuse(vec3 normal, vec3 lightDir, vec3 diffuse)
{
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 final = diffuse * (diff * vec3(texture(material.texture_diffuse1,TexCoords)));
    return final;
}

vec3 GetSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specular)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 final = specular * (spec * (vec3(texture(material.texture_specular1,TexCoords))));
    return final;
}