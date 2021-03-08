#version 460 core
out vec4 FragColor;

in vec3 TexPos;

uniform samplerCube skybox;
uniform vec3 tintColor;

void main() {
  vec3 color = vec3(texture(skybox, TexPos)) * tintColor;
  FragColor = vec4(color, 1.0f);
}
