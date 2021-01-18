#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.25f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.75f;
    float shininess = 256;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular)* objectColor;
    FragColor=vec4(result, 1.0f);
}