#version 460 core
out vec4 FragColor;

uniform vec3 sourceColor;
float gamma = 2.2f;

void main() {
  vec3 mainColor = pow(sourceColor.rgb, vec3(1.0 / (gamma)));
  FragColor = vec4(mainColor, 1.0f);
}