#include <gui/ObjectUI.h>

ShaderUI::ShaderUI()
{
}

ShaderUI::ShaderUI(std::string vertexPath, std::string fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
}

TextureUI::TextureUI()
{
}

TextureUI::TextureUI(std::string diffusePath, std::string specularPath,
                     std::string emmisionPath, std::string normalPath,
                     std::string depthPath)
{
    this->diffusePath = diffusePath;
    this->specularPath = specularPath;
    this->emmisionPath = emmisionPath;
    this->normalPath = normalPath;
    this->depthPath = depthPath;
}