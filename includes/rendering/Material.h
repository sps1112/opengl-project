#ifndef MATERIAL_H
#define MATERIAL_H

#include <utility/Utils.h>
#include <utility/CustomMath.h>
#include <rendering/Texture.h>
#include <config.h>

using Colorf = Vec3;

// Types of Shaders
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

extern std::string shader_directory;

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
const Material defaultMat2D(Colorf(DEFAULT_SHADER_COLOR), Colorf(COLOR_BLACK), COLOR_2D);
const Material defaultMat3D(Colorf(DEFAULT_SHADER_COLOR), Colorf(COLOR_BLACK), COLOR_3D);

std::string GetVSPath(SHADER_TYPE type);
std::string GetFSPath(SHADER_TYPE type);

#endif