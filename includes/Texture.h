#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned int LoadTextureFromPath(const char *path,
                                 bool gammaCorrection = false, bool isDiffuse = true, bool toClamp = false);
unsigned int GenerateTexture();
void BindTexture(unsigned int id);
void UnBindTexture();
void SetActiveTexture(int index);
void BindCubemap(unsigned int id);
unsigned int LoadCubemapFromPath(std::string directory, std::string extension);
#endif