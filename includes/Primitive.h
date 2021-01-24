#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <Shader.h>

#include <string>
#include <vector>

struct pVertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct pTexture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Primitive
{
public:
    vector<pVertex> vertices;
    vector<unsigned int> indices;
    vector<pTexture> textures;
    unsigned int VAO;
    Primitive(const char *path)
    {
        SetupData();
        SetupPrimitive();
    }

    void Draw(Shader &shader)
    {
        glActiveTexture(GL_TEXTURE0);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    // render data
    unsigned int VBO, EBO;
    void SetupData()
    {
    }

    void SetupPrimitive()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // free memory
        glBindVertexArray(0);
    }
};
#endif