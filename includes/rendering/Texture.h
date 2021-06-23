#ifndef TEXTURE_H
#define TEXTURE_H

// Header declarations
#include <external/glad/glad.h>
#include <external/GLFW/glfw3.h>
#include <external/stb_image.h>
#include <config.h>
#include <utility/FileSystem.h>

// Standard Headers
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
extern std::string textureTypeStrings[5];

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
TEXTURE_TEMPLATES get_tex_template(int i);

extern std::string textureFolderPath;
extern std::string textureFileNames[17];
extern TEXTURE_TYPE templateTextureTypes[17];

// Texture Struct
struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

// Texture Loading

// Loads a Texture from type and path
Texture load_texture(TEXTURE_TYPE type, const std::string &path, bool isDiffuse = true, bool toClamp = false);
// Loads a Texture and returns id
unsigned int load_texture_from_path(const char *path, bool isDiffuse = true, bool toClamp = false);
// Loads a Texture and returns id
unsigned int load_texture_from_path(const std::string &path, bool isDiffuse = true, bool toClamp = false);
// Gets Texture from a defined Template
Texture get_from_template(TEXTURE_TEMPLATES template_, bool toClamp = false);

// Generating Textures

// Generates a Texture and returns its id
unsigned int generate_texture();
// Binds a Texture to current memory
void bind_texture(unsigned int id);
// UnBinds the currently bound texture
void unbind_texture();
// Sets Active texture via id
void set_active_texture(int index);

// CubeMap Textures

// Binds cubemap to memory
void bind_cubemap(unsigned int id);
// Loads cubemap from path
unsigned int load_cubemap_from_path(std::string directory, std::string extension);

#endif // !TEXTURE_H
