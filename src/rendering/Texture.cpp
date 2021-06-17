#include <rendering/Texture.h>

std::string texture_type_strings[] = {
    "texture_diffuse",
    "texture_specular",
    "texture_emmision",
    "texture_normal",
    "texture_height"};

TEXTURE_TEMPLATES GetTexTemplate(int i)
{
    return static_cast<TEXTURE_TEMPLATES>(i);
}

std::string texture_folder_path = "resources/textures/";

std::string texture_file_name[17] = {
    "awesomeface.png",
    "wall.jpg",
    "brickwall.jpg",
    "brickwall_normal.jpg",
    "bricks2.jpg",
    "bricks2_normal.jpg",
    "bricks2_disp.jpg",
    "container.jpg",
    "container2.png",
    "container2_specular.png",
    "container2_specular_color.png",
    "marble.jpg",
    "metal.png",
    "wood.png",
    "window.png",
    "grass.png",
    "matrix.jpg"};

TEXTURE_TYPE template_textype[17] = {
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_NORMAL,
    TEXTURE_DIFFUSE,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_SPECULAR,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_DIFFUSE,
    TEXTURE_EMMISION};

Texture get_from_template(TEXTURE_TEMPLATES template_, bool toClamp)
{
    std::string path = texture_folder_path + texture_file_name[template_];
    return LoadTexture(template_textype[template_], FileSystem::getPath(path), (template_textype[template_] == TEXTURE_DIFFUSE), toClamp);
}

Texture LoadTexture(TEXTURE_TYPE type, const std::string &path, bool isDiffuse, bool toClamp)
{
    Texture newTexture;
    newTexture.id = LoadTextureFromPath((path.c_str()), isDiffuse, toClamp);
    newTexture.type = texture_type_strings[type];
    newTexture.path = path;
    return newTexture;
}

unsigned int LoadTextureFromPath(const char *path, bool isDiffuse, bool toClamp)
{
    unsigned int textureID = GenerateTexture();
    bool gammaCorrection = false;
#if GAMMA_CORRECTION_ENABLED
    gammaCorrection = true;
#endif
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    // setup data
    if (data)
    {
        GLenum format;
        GLenum otherFormat;
        if (nrComponents == 1)
        {
            format = GL_RED;
            otherFormat = format;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
            otherFormat = (gammaCorrection && isDiffuse) ? GL_SRGB : format;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
            otherFormat = (gammaCorrection && isDiffuse) ? GL_SRGB_ALPHA : format;
        }
        BindTexture(textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, otherFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        ((toClamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        ((toClamp) ? GL_CLAMP_TO_EDGE : GL_REPEAT));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

unsigned int LoadTextureFromPath(const std::string &path, bool isDiffuse, bool toClamp)
{
    return LoadTextureFromPath(path.c_str(), isDiffuse, toClamp);
}

unsigned int GenerateTexture()
{
    unsigned int id;
    glGenTextures(1, &id);
    return id;
}

void BindTexture(unsigned int id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void UnBindTexture()
{
    glActiveTexture(GL_TEXTURE0);
}

void SetActiveTexture(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

void BindCubemap(unsigned int id)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

unsigned int LoadCubemapFromPath(std::string directory, std::string extension)
{
    unsigned int textureID = GenerateTexture();
    BindCubemap(textureID);
    std::string names[6] = {
        "right", "left",
        "top", "bottom",
        "front", "back"};
    stbi_set_flip_vertically_on_load(false);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        std::string fileName = directory + "/" + names[i] + extension;
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << fileName << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(true);
    return textureID;
}
