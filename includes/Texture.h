#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

enum TEXTURE_TYPE
{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_EMMISION,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

Texture LoadTexture(TEXTURE_TYPE type, const std::string &path,
                    bool gammaCorrection = false, bool isDiffuse = true, bool toClamp = false);
unsigned int LoadTextureFromPath(const char *path,
                                 bool gammaCorrection = false, bool isDiffuse = true, bool toClamp = false);
unsigned int LoadTextureFromPath(const std::string &path,
                                 bool gammaCorrection = false, bool isDiffuse = true, bool toClamp = false);
unsigned int GenerateTexture();
void BindTexture(unsigned int id);
void UnBindTexture();
void SetActiveTexture(int index);
void BindCubemap(unsigned int id);
unsigned int LoadCubemapFromPath(std::string directory, std::string extension);
#endif