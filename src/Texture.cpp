#include <Texture.h>

unsigned int LoadTextureFromPath(const char *path,
                                 bool gammaCorrection, bool isDiffuse, bool toClamp)
{
    unsigned int textureID = GenerateTexture();

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
