#ifndef TEXTURE_H
#define TEXTURE_H

// Header declarations
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

// Texture typestrings
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

// Loads a Texture from type and path
Texture LoadTexture(TEXTURE_TYPE type, const std::string &path, bool isDiffuse = true, bool toClamp = false);
// Loads a Texture and returns id
unsigned int LoadTextureFromPath(const char *path, bool isDiffuse = true, bool toClamp = false);
// Loads a Texture and returns id
unsigned int LoadTextureFromPath(const std::string &path, bool isDiffuse = true, bool toClamp = false);

// Generating Textures

// Generates a Texture and returns its id
unsigned int GenerateTexture();
// Binds a Texture to current memory
void BindTexture(unsigned int id);
// UnBinds the currently bound texture
void UnBindTexture();
// Sets Active texture via id
void SetActiveTexture(int index);

// CubeMap Textures

// Binds cubemap to memory
void BindCubemap(unsigned int id);
// Loads cubemapr from path
unsigned int LoadCubemapFromPath(std::string directory, std::string extension);

#endif // !TEXTURE_H