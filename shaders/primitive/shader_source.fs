#version 330 core
out vec4 FragColor;

uniform vec3 sourceColor;

void main()
{
    FragColor=vec4(sourceColor,1.0f);
}