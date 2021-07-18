#ifndef MATERIAL_H
#define MATERIAL_H

// Header declarations
#include <utility/CustomMath.h>
#include <rendering/Texture.h>
#include <config.h>

// Color Data types

// A color represented as a Vector3 i.e. (R,G,B)
using ColorF = Vec3;
// A color representeed as a Vector4 i.e. (R,G,B,A)
using ColorA = Vec4;

// Types of Template Shaders
enum SHADER_TYPE
{
    COLOR_2D_SHADER,
    TEXTURE_2D_SHADER,
    LIGHT_SHADER,
    COLOR_3D_SHADER,
    TEXTURE_3D_SHADER,
    MATERIAL_3D_SHADER,
    MODEL_3D_SHADER,
    SCENE_3D_SHADER,
    FBO_SHADER,
    SKYBOX_SHADER,
    BLENDING_SHADER,
    REFLECTION_SHADER,
    REFRACATION_SHADER,
};

// Shader folder directory path
extern std::string shaderDirectory;

// The Template Shader struct including all the data for a Shader's Template
struct TemplateShader
{
    SHADER_TYPE type; // The type of shader template
    std::string name; // Name of the shader file
    // Template Struct Constructor
    TemplateShader(SHADER_TYPE type_, std::string fileName) : type(type_), name(fileName) {}

    // Gets Path to the Vertex Shader file
    std::string get_vs_path()
    {
        return (shaderDirectory + name + ".vs");
    }

    // Gets Path to the Fragment Shader file
    std::string get_fs_path()
    {
        return (shaderDirectory + name + ".fs");
    }
};

// All the Templates of the defined Shaders
extern TemplateShader templateShaders[13];

// Material field struct
struct MatField
{
    ColorF col; // The Color of the field
    int texID;  // The ID of texture
    // Default MatField Constructor
    MatField() : col(COLOR_GRAY), texID(0) {}
    // Matfield Struct Constructor
    MatField(ColorF col_) : col(col_), texID(0) {}
    TEXTURE_TEMPLATES get_template()
    {
        return get_tex_template(texID);
    }
};

// Material Struct
struct Material
{
    MatField albedo;   // The albedo/diffuse channel for the Material
    MatField specular; // The specular channel for the Material
    MatField emmision; // The emission channel for the Material
    SHADER_TYPE type;  // The type of Shader file
    // Default Material struct Constructor
    Material()
    {
        albedo = MatField();
        emmision = MatField(COLOR_BLACK);
        type = COLOR_2D_SHADER;
    }
    // Material struct Constructor
    Material(ColorF albedo_, ColorF emmision_, SHADER_TYPE type_)
    {
        albedo = MatField(albedo_);
        emmision = MatField(emmision_);
        type = type_;
    }
    // Material Copy Constructor
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
    STANDARD_3D_MAT,
    MODEL_3D_MAT,
    SCENE_3D_MAT,
};

// Template Material instances
extern Material material_template[8];

#endif // !MATERIAL_H
