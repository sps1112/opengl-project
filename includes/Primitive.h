#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <FileReader.h>
#include <Utils.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Vertex2D
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;
};

struct Vertex3D
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Color;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct pTexture
{
    unsigned int id;
    std::string type;
};

glm::vec3 GetVec3(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex);
glm::vec2 GetVec2(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex);
bool CheckNextLine(char *fileData, int lineStartIndex);

class Primitive
{
public:
    vector<Vertex2D> vertices2D;
    vector<Vertex3D> vertices3D;
    vector<unsigned int> indices;
    vector<pTexture> textures;
    unsigned int VAO;
    Primitive(const char *path)
    {
        SetupData(path);
        SetupPrimitive();
    }

    void SetupTextures(vector<pTexture> textures)
    {
        this->textures = textures;
    }

    void Draw(Shader &shader)
    {
        unsigned int diffusedNR = 1;
        unsigned int specularNR = 1;
        unsigned int normalNR = 1;
        unsigned int heightNR = 1;
        unsigned int emmisionNR = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
            {
                number = std::to_string(diffusedNR++);
            }
            else if (name == "texture_specular")
            {
                number = std::to_string(specularNR++);
            }
            else if (name == "texture_normal")
            {
                number = std::to_string(normalNR++);
            }
            else if (name == "texture_height")
            {
                number = std::to_string(heightNR++);
            }
            else if (name == "texture_emmision")
            {
                number = std::to_string(emmisionNR++);
            }
            shader.setInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        if (is2D)
        {
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
        glBindVertexArray(0);
    }

private:
    // render data
    unsigned int VBO, EBO;
    bool is2D;
    int vertexCount;
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

        int pathLength = GetCharArrayLength(path);
        // std::cout << path << std::endl;
        char *extension = GetLine(ConvertToCharArray(path), pathLength - 4);
        if (CompareCharArray(extension, ConvertToCharArray(".2d")))
        {
            is2D = true;
        }
        else if (CompareCharArray(extension, ConvertToCharArray(".3d")))
        {
            is2D = false;
        }
        vertexCount = GetVertexCount(ConvertToCharArray(fileData));
        ProcessData(ConvertToCharArray(fileData));
    }

    int GetVertexCount(char *fileData)
    {
        int count = 0;
        int lineStartIndex = 0;
        char firstChar = *(fileData + 0);
        char *lineI = GetLine(fileData, lineStartIndex);
        while (count == 0 || *(lineI + 0) != firstChar)
        {
            lineStartIndex = GetLineStartIndex(fileData, lineStartIndex);
            lineI = GetLine(fileData, lineStartIndex);
            count++;
        }
        count -= 2;
        // std::cout << "Count is " << count << std::endl;
        return count;
    }

    void ProcessData(char *fileData)
    {
        int lineStartIndex = 0;
        char *lineI = GetLine(fileData, lineStartIndex);
        char targetChar = ' ';
        char *whitespace = ConvertToCharArray(std::string(" ").c_str());
        if (is2D)
        {
            Vertex2D vertexArray[vertexCount];
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].Position = GetVec3(fileData, 'v', is2D, lineStartIndex, i);
            }
            // Vertex Set
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].Color = GetVec3(fileData, 'c', is2D, lineStartIndex, i);
            }
            // Color Set
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].TexCoord = GetVec2(fileData, 't', is2D, lineStartIndex, i);
            }
            // Texture Set
            for (int i = 0; i < vertexCount; i++)
            {
                vertices2D.push_back(vertexArray[i]);
            }
            // Setting Indices
            lineStartIndex = SkipLines(fileData, lineStartIndex, 3);
            int i = 0;
            while (((char)fileData[lineStartIndex]) != '\0')
            {
                lineI = GetLine(fileData, lineStartIndex);
                // std::cout << lineI << std::endl;

                std::string target = "i " + std::to_string(i) + " ";
                char *val[3];
                int endIndex = GetEndIndexString(lineI, ConvertToCharArray(target.c_str()));
                for (int i = 0; i < 3; i++)
                {
                    int startIndex = GetOtherCharIndex(lineI, endIndex, targetChar);
                    lineI = GetLine(lineI, startIndex);
                    if (i == 2)
                    {
                        val[i] = lineI;
                    }
                    else
                    {
                        endIndex = GetEndIndexString(lineI, whitespace);
                        val[i] = GetLinePart(lineI, 0, endIndex - 1);
                    }
                }

                unsigned int a, b, c;
                a = (unsigned int)ConvertToFloat(val[0]);
                b = (unsigned int)ConvertToFloat(val[1]);
                c = (unsigned int)ConvertToFloat(val[2]);
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                i++;
            }
            /*
            // Indices Set
            if (((char)fileData[lineStartIndex]) == '\0')
            {
                std::cout << "End" << std::endl;
            }
            else
            {
                std::cout << "Not end " << ((char)fileData[lineStartIndex]) << "x" << std::endl;
            }
            */
        }
        else
        {
            Vertex3D vertexArray[vertexCount];
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].Position = GetVec3(fileData, 'v', is2D, lineStartIndex, i);
            }
            // Log("Position set");
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].Normal = GetVec3(fileData, 'n', is2D, lineStartIndex, i);
            }
            // Log("Normal set");
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].TexCoord = GetVec2(fileData, 't', is2D, lineStartIndex, i);
            }
            // Log("Texture coords set");
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            for (int i = 0; i < vertexCount; i++)
            {
                lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                vertexArray[i].Color = GetVec3(fileData, 'c', is2D, lineStartIndex, i);
            }
            // Log("Color set");
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            if (CheckNextLine(fileData, lineStartIndex))
            {
                for (int i = 0; i < vertexCount; i++)
                {
                    lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                    vertexArray[i].Tangent = GetVec3(fileData, 'T', is2D, lineStartIndex, i);
                }
                // Log("Tangent set");
            }
            lineStartIndex = SkipLines(fileData, lineStartIndex, 2);
            if (CheckNextLine(fileData, lineStartIndex))
            {
                for (int i = 0; i < vertexCount; i++)
                {
                    lineStartIndex = SkipLines(fileData, lineStartIndex, 1);
                    vertexArray[i].Bitangent = GetVec3(fileData, 'b', is2D, lineStartIndex, i);
                }
                // Log("Bitangent set");
            }
            for (int i = 0; i < vertexCount; i++)
            {
                vertices3D.push_back(vertexArray[i]);
            }
        }
    }

    void SetupPrimitive()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        if (is2D)
        {
            glBufferData(GL_ARRAY_BUFFER, vertices2D.size() * sizeof(Vertex2D), &vertices2D[0], GL_STATIC_DRAW);
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, vertices3D.size() * sizeof(Vertex3D), &vertices3D[0], GL_STATIC_DRAW);
        }

        if (is2D)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        }

        if (is2D)
        {
            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);
            // vertex color
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, Color));
            // vertex Texture coordinates
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, TexCoord));
        }
        else
        {
            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Normal));
            // vertex Texture coordinates
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)offsetof(Vertex3D, TexCoord));
            // vertex colors
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Color));
            // vertex Tangents
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Tangent));
            // vertex Bitangents
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void *)offsetof(Vertex3D, Bitangent));
        }
        // free memory
        glBindVertexArray(0);
    }
};

glm::vec3 GetVec3(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex)
{
    char targetChar = ' ';
    char *whitespace = ConvertToCharArray(std::string(" ").c_str());
    char *lineI = GetLine(fileData, lineStartIndex);
    std::string target = std::string(1, itemType) + " ";
    if (!is2D)
    {
        if (vertexIndex < 10)
        {
            target += "0";
        }
    }
    target += std::to_string(vertexIndex) + " ";
    char *val[3];
    int endIndex = GetEndIndexString(lineI, ConvertToCharArray(target.c_str()));
    for (int i = 0; i < 3; i++)
    {
        int startIndex = GetOtherCharIndex(lineI, endIndex, targetChar);
        lineI = GetLine(lineI, startIndex);
        if (i == 2)
        {
            val[i] = lineI;
        }
        else
        {
            endIndex = GetEndIndexString(lineI, whitespace);
            val[i] = GetLinePart(lineI, 0, endIndex - 1);
        }
    }
    glm::vec3 a;
    a.x = ConvertToFloat(val[0]);
    a.y = ConvertToFloat(val[1]);
    a.z = ConvertToFloat(val[2]);
    return a;
}

glm::vec2 GetVec2(char *fileData, char itemType, bool is2D, int lineStartIndex, int vertexIndex)
{
    char targetChar = ' ';
    char *whitespace = ConvertToCharArray(std::string(" ").c_str());
    char *lineI = GetLine(fileData, lineStartIndex);
    std::string target = std::string(1, itemType) + " ";
    if (!is2D)
    {
        if (vertexIndex < 10)
        {
            target += "0";
        }
    }
    target += std::to_string(vertexIndex) + " ";
    char *val[2];
    int endIndex = GetEndIndexString(lineI, ConvertToCharArray(target.c_str()));
    for (int i = 0; i < 2; i++)
    {
        int startIndex = GetOtherCharIndex(lineI, endIndex, targetChar);
        lineI = GetLine(lineI, startIndex);
        if (i == 1)
        {
            val[i] = lineI;
        }
        else
        {
            endIndex = GetEndIndexString(lineI, whitespace);
            val[i] = GetLinePart(lineI, 0, endIndex - 1);
        }
    }
    glm::vec2 a;
    a.x = ConvertToFloat(val[0]);
    a.y = ConvertToFloat(val[1]);
    return a;
}

bool CheckNextLine(char *fileData, int lineStartIndex)
{
    char testChar = ((char)fileData[SkipLines(fileData, lineStartIndex, 1)]);
    bool status = (testChar != ' ' && testChar != '\0' && testChar != '\n');
    return status;
}
#endif