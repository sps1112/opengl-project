#ifndef OBJECTUI_H
#define OBJECTUI_H

#include <Shader.h>
#include <Texture.h>

#include <iostream>

class ShaderUI
{
public:
    std::string vertexPath;
    std::string fragmentPath;
    ShaderUI();
    ShaderUI(std::string vertexPath, std::string fragmentPath);
};

class TextureUI
{
public:
    std::string diffusePath;
    std::string specularPath;
    std::string emmisionPath;
    std::string normalPath;
    std::string depthPath;
    TextureUI();
    TextureUI(std::string diffusePath, std::string specularPath = "",
              std::string emmisionPath = "", std::string normalPath = "",
              std::string depthPath = "");
};

#endif