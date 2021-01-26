#version 330 core
out vec4 FragColor;

in vec3 FinalColor;

void main()
{
    FragColor=vec4(FinalColor, 1.0f);
}