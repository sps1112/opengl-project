#version 460 core
struct Material {
  sampler2D texture_diffuse1;
};
uniform Material material;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

void main() {
  vec3 finalColor =
      vec3(texture(material.texture_diffuse1, TexCoord)) * ourColor;
  FragColor = vec4(finalColor, 1.0f);
}