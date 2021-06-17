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

// Material field struct
struct MatField
{
    Colorf col;
    TEXTURE_TEMPLATES tex;
    int texID;
    MatField() : col(COLOR_GRAY), texID(0) {}
    MatField(Colorf col_) : col(col_), texID(0) {}
};

// Material Struct
struct Material
{
    MatField albedo;
    MatField specular;
    MatField emmision;
    SHADER_TYPE type;

    Material()
    {
        albedo = MatField();
        emmision = MatField(COLOR_BLACK);
        type = COLOR_2D;
    }

    Material(Colorf albedo_, Colorf emmision_, SHADER_TYPE type_)
    {
        albedo = MatField(albedo_);
        emmision = MatField(emmision_);
        type = type_;
    }

    Material(const Material &mat)
    {
        albedo = mat.albedo;
        emmision = mat.emmision;
        type = mat.type;
    }
};

// Template Material types
enum MATERIAL_TEMPLATES
{
    COLOR_2D_MAT,
    COLOR_3D_MAT,
    LIGHT_MAT,
    TEX_2D_MAT,
    TEX_3D_MAT,
};

// Template Material instances
extern Material material_template[5];

#endif // !MATERIAL_H