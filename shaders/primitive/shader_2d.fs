#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
};
uniform Material material;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;


void main()
{
	FragColor = texture(material.texture_diffuse1, TexCoord) * vec4(ourColor, 1.0f);
}
