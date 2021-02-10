#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 ourColor;

void main() { FragColor = vec4(ourColor, 1.0f); }
