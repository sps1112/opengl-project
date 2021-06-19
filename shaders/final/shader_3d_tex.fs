#version 460 core
struct Material {
  sampler2D texture_diffuse1;
};
uniform Material material;
uniform vec3 matColor;
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

void main() {
      vec3 finalColor =
      vec3(texture(material.texture_diffuse1, TexCoord)) * matColor;
    FragColor = vec4(finalColor, 1.0f); }
