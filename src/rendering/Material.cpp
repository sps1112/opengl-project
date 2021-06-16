#include <rendering/Material.h>

std::string GetVSPath(SHADER_TYPE type)
{
    return (shader_directory + shader_type_strings[type] + ".vs");
}

std::string GetFSPath(SHADER_TYPE type)
{
    return (shader_directory + shader_type_strings[type] + ".fs");
}

std::string shader_type_strings[] = {
    "shader_2d_color",
    "shader_2d_tex",
    "shader_light",
    "shader_3d_color",
    "shader_3d_mat",
    "shader_3d_tex",
    "shader_3d_model",
    "shader_fbo",
    "shader_skybox",
    "shader_blend",
    "shader_reflect",
    "shader_refract"};

std::string shader_directory = "shaders/final/";