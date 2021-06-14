#ifndef MATERIAL_H
#define MATERIAL_H

#include <utility/Utils.h>
#include <utility/CustomMath.h>
#include <rendering/Texture.h>

using Colorf = Vec3;

// Types of Shaders
enum SHADER_TYPE
{
    COLOR_2D,
    TEXTURE_2D,
    LIGHT_SHADER,
    COLOR_3D,
    TEXTURE_3D,
    MODEL_3D,
    FBO_SHADER,
    SKYBOX_SHADER,
    BLENDING_SHADER,
    REFLECTION_SHADER,
    REFRACATION_SHADER,
};

// File Name for Shader Files
const std::string shader_type_strings[] = {
    "shader_2d_color",
    "shader_2d_tex",
    "shader_light",
    "shader_3d_color",
    "shader_3d_tex",
    "shader_3d_model",
    "shader_fbo",
    "shader_skybox",
    "shader_blend",
    "shader_reflect",
    "shader_refract"};

// Material Struct
struct Material
{
    Colorf albedo;
    Colorf emmision;
    SHADER_TYPE type;

    Material(Colorf albedo_, Colorf emmision_, SHADER_TYPE type_)
        : albedo(albedo_), emmision(emmision_), type(type_) {}
};

#endif