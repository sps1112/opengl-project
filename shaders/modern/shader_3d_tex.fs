#version 460 core
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  sampler2D texture_emmision1;
  float shininess;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 ourColor;

uniform Material material;

void main() {
  FragColor = texture(material.texture_diffuse1, TexCoord) * vec4(1.0f);
}
