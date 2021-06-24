#include <rendering/Texture.h>

// Texture typestrings
std::string textureTypeStrings[] = {
    "texture_diffuse",
    "texture_specular",
    "texture_emmision",
    "texture_normal",
    "texture_height"};

TEXTURE_TEMPLATES get_tex_template(int i)
{
    return (static_cast<TEXTURE_TEMPLATES>(i));
}

// Path to the texture folder
std::string textureFolderPath = "resources/textures/";

// File names of the defined texture templates
std::string textureFileNames[17] = {
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

// Type of texture file for each defined template
TEXTURE_TYPE templateTextureTypes[17] = {
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

unsigned int load_texture_from_path(const char *path, bool isDiffuse, bool toClamp)
{
    unsigned int textureID = generate_texture();
    bool gammaCorrection = false;
#if GAMMA_CORRECTION_ENABLED
    gammaCorrection = true;
#endif
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    // Setup texture data
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
        bind_texture(textureID);
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

unsigned int load_texture_from_path(const std::string &path, bool isDiffuse, bool toClamp)
{
    return load_texture_from_path(path.c_str(), isDiffuse, toClamp);
}

Texture load_texture(TEXTURE_TYPE type, const std::string &path, bool isDiffuse, bool toClamp)
{
    Texture newTexture;
    newTexture.id = load_texture_from_path((path.c_str()), isDiffuse, toClamp);
    newTexture.type = textureTypeStrings[type];
    newTexture.path = path;
    return newTexture;
}

Texture get_from_template(TEXTURE_TEMPLATES template_, bool toClamp)
{
    std::string path = textureFolderPath + textureFileNames[template_];
    return load_texture(templateTextureTypes[template_], FileSystem::get_path(path), (templateTextureTypes[template_] == TEXTURE_DIFFUSE), toClamp);
}

unsigned int generate_texture()
{
    unsigned int id;
    glGenTextures(1, &id);
    return id;
}

void bind_texture(unsigned int id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void unbind_texture()
{
    glActiveTexture(GL_TEXTURE0);
}

void set_active_texture(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

void bind_cubemap(unsigned int id)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

unsigned int load_cubemap_from_path(std::string directory, std::string extension)
{
    unsigned int textureID = generate_texture();
    bind_cubemap(textureID);
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
