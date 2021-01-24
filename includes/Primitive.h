#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <FileReader.h>

#include <iostream>
#include <fstream>
#include <sstream>
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
        SetupData(path);
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
    void SetupData(const char *path)
    {
        std::string pathString;
        std::ifstream file;
        file.open(path);
        std::stringstream statsStream;
        statsStream << file.rdbuf();
        file.close();
        pathString = statsStream.str();
        const char *fileData = pathString.c_str();
        std::cout << path << std::endl;
        int pathLength = GetCharArrayLength(path);
        std::cout << pathLength << std::endl;
        std::cout << GetCharArrayLength(".2d") << std::endl;
        int endIndex1 = GetStartIndexString(path, ".2d");
        std::cout << "Index 1 is " << endIndex1 << std::endl;
        int endIndex2 = GetStartIndexString(path, ".3d");
        std::cout << "Index 2 is " << endIndex2 << std::endl;
        if (GetEndIndexString(path, ".2d") == GetLineEndIndex(path, 0))
        {
            std::cout << "Yes " << GetEndIndexString(path, ".2d") << " " << GetLineEndIndex(path, 0) << std::endl;
        }
        else
        {
            std::cout << "No " << GetEndIndexString(path, ".2d") << " " << GetLineEndIndex(path, 0) << std::endl;
        }
        char *Line1 = GetLine(fileData, 0);
        std::cout << Line1 << std::endl;
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