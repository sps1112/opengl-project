#ifndef TEXTURE_H
#define TEXTURE_H

#include <external/glad/glad.h>
#include <external/GLFW/glfw3.h>
#include <external/stb_image.h>
#include <config.h>

#include <iostream>

// Types of Textures
enum TEXTURE_TYPE
{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_EMMISION,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
};

// Corresponding strings

const std::string texture_type_strings[] = {
    "texture_diffuse",
    "texture_specular",
    "texture_emmision",
    "texture_normal",
    "texture_height"};

// Texture Struct
struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

// Texture Loading

Texture LoadTexture(TEXTURE_TYPE type, const std::string &path, bool isDiffuse = true, bool toClamp = false);
unsigned int LoadTextureFromPath(const char *path, bool isDiffuse = true, bool toClamp = false);
unsigned int LoadTextureFromPath(const std::string &path, bool isDiffuse = true, bool toClamp = false);

// Generating Textures

unsigned int GenerateTexture();
void BindTexture(unsigned int id);
void UnBindTexture();
void SetActiveTexture(int index);

// CubeMap Textures

void BindCubemap(unsigned int id);
unsigned int LoadCubemapFromPath(std::string directory, std::string extension);

#endif