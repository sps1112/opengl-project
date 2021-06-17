#ifndef TEXTURE_H
#define TEXTURE_H

// Header declarations
#include <external/glad/glad.h>
#include <external/GLFW/glfw3.h>
#include <external/stb_image.h>
#include <config.h>
#include <utility/FileSystem.h>

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
extern std::string texture_type_strings[5];

// Template Textures
enum TEXTURE_TEMPLATES
{
    FACE_TEX,
    WALL_TEX,
    BRICK_WALL_TEX,
    BRICK_WALL_NORMAL,
    REDBRICK_WALL_TEX,
    REDBRICK_WALL_NORMAL,
    REDBRICK_WALL_DIS,
    CONTAINER_TEX,
    CONTAINER_BORDER_TEX,
    CONTAINER_BORDER_SPEC,
    CONTAINER_BORDER_SPEC_COL,
    MARBLE_TEX,
    METAL_TEX,
    WOOD_TEX,
    WINDOW_TEX,
    GRASS_TEX,
    MATRIX_TEX,
};
TEXTURE_TEMPLATES GetTexTemplate(int i);

extern std::string texture_folder_path;
extern std::string texture_file_name[17];
extern TEXTURE_TYPE template_textype[17];

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

Texture get_from_template(TEXTURE_TEMPLATES template_, bool toClamp = false);
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