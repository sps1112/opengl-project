#version 460 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emmision;
  float shininess;
};

struct Light {
  vec3 direction;

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

void main() {
  // Ambient Lighting
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffused Lighting
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse =
      light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

  // Specular Lighting
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular =
      light.specular * (spec * (vec3(texture(material.specular, TexCoords))));

  // Emmision Lighting
  vec3 emmision = vec3(0.0f);
  if (texture(material.specular, TexCoords).r == 0.0) {
    emmision = vec3(texture(material.emmision, TexCoords));
  }

  // Resultant Lighting
  vec3 result = (ambient + diffuse + specular + emmision);
  FragColor = vec4(result, 1.0f);
}