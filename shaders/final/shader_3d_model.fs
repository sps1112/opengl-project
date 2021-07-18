#version 460 core
struct Material {
  sampler2D texture_diffuse1;
};
uniform Material material;
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
uniform vec3 matColor;

void main() {
  vec3 result = vec3(0.0f);
  result+=vec3(texture(material.texture_diffuse1, TexCoords))* matColor;
  FragColor = vec4(result, 1.0f);
}
