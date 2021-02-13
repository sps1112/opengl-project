#version 460 core
struct Material {
  sampler2D texture_diffuse1;
};
uniform Material material;
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 ourColor;

uniform bool toDiscard;
void main() {
  vec4 texColor = texture(material.texture_diffuse1, TexCoords);
  if (toDiscard) {
    if (texColor.a < 0.1f) {
      discard;
    }
  }
  FragColor = texColor;
}