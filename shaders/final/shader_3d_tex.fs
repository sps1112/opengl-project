#version 460 core
out vec4 FragColor;

uniform vec3 matColor;

in vec3 Normal;
in vec3 FragPos;

void main() { FragColor = vec4(matColor, 1.0f); }
