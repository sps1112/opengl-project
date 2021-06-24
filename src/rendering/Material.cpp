#include <rendering/Material.h>

// Shader folder directory path
std::string shaderDirectory = "shaders/final/";

// All the Templates of the defined Shaders
TemplateShader templateShaders[] = {
    TemplateShader(COLOR_2D_SHADER, "shader_2d_color"),
    TemplateShader(TEXTURE_2D_SHADER, "shader_2d_tex"),
    TemplateShader(LIGHT_SHADER, "shader_light"),
    TemplateShader(COLOR_3D_SHADER, "shader_3d_color"),
    TemplateShader(TEXTURE_3D_SHADER, "shader_3d_tex"),
    TemplateShader(MATERIAL_3D_SHADER, "shader_3d_mat"),
    TemplateShader(MODEL_3D_SHADER, "shader_3d_model"),
    TemplateShader(SCENE_3D_SHADER, "shader_3d_scene"),
    TemplateShader(FBO_SHADER, "shader_fbo"),
    TemplateShader(SKYBOX_SHADER, "shader_skybox"),
    TemplateShader(BLENDING_SHADER, "shader_blend"),
    TemplateShader(REFLECTION_SHADER, "shader_reflect"),
    TemplateShader(REFRACATION_SHADER, "shader_refract"),
};

// Template Material instances
Material material_template[] = {
    Material(ColorF(DEFAULT_SHADER_COLOR), ColorF(COLOR_BLACK), COLOR_2D_SHADER),
    Material(ColorF(DEFAULT_SHADER_COLOR), ColorF(COLOR_BLACK), COLOR_3D_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), LIGHT_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), TEXTURE_2D_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), TEXTURE_3D_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), MATERIAL_3D_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), MODEL_3D_SHADER),
    Material(ColorF(COLOR_WHITE), ColorF(COLOR_BLACK), SCENE_3D_SHADER),
};
