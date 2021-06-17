#ifndef MATERIAL_H
#define MATERIAL_H

// Header declarations
#include <utility/Utils.h>
#include <utility/CustomMath.h>
#include <rendering/Texture.h>
#include <config.h>

// Color data types
using Colorf = Vec3;

// Types of Template Shaders
enum SHADER_TYPE
{
    COLOR_2D,
    TEXTURE_2D,
    LIGHT_SHADER,
    COLOR_3D,
    COLR_3D_MAT,
    TEXTURE_3D,
    MODEL_3D,
    FBO_SHADER,
    SKYBOX_SHADER,
    BLENDING_SHADER,
    REFLECTION_SHADER,
    REFRACATION_SHADER,
};

// File Name for Shader Files
extern std::string shader_type_strings[12];
// Shader folder directory
extern std::string shader_directory;

// Get Vertex Shader file Path
std::string GetVSPath(SHADER_TYPE type);
// Get Fragment Shader file Path
std::string GetFSPath(SHADER_TYPE type);

// Material Struct
struct Material
{
    Colorf albedo;
    Colorf emmision;
    SHADER_TYPE type;
    Material()
        : albedo(COLOR_GRAY), emmision(COLOR_BLACK), type(COLOR_2D) {}
    Material(Colorf albedo_, Colorf emmision_, SHADER_TYPE type_)
        : albedo(albedo_), emmision(emmision_), type(type_) {}
    Material(const Material &mat)
        : albedo(mat.albedo), emmision(mat.emmision), type(mat.type) {}
};

enum MATERIAL_TEMPLATES
{
    COLOR_2D_MAT,
    COLOR_3D_MAT,
    LIGHT_MAT,
};

extern Material material_template[3];

#endif // !MATERIAL_H