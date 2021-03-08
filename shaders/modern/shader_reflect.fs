#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 ourColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform bool toRefract;

float ratio = 1.00 / 1.52;

void main() {
  vec3 I = normalize(FragPos - cameraPos);
  vec3 R = reflect(I, normalize(Normal));
  if (toRefract) // Refracts skybox
  {
    R = refract(I, normalize(Normal), ratio);
  }
  FragColor = vec4(texture(skybox, R).rgb, 1.0);
}