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
void SetActiveTexture(int index);
void BindTexture(unsigned int id);
void UnBindTexture();

#endif