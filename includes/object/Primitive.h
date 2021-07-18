#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <external/glad/glad.h>

#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>

#include <rendering/Shader.h>
#include <utility/FileReader.h>
#include <utility/Utils.h>
#include <rendering/Renderer.h>
#include <rendering/Texture.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Vertex2D
{
    glm::vec3 position;
    glm::vec3 Color;
    glm::vec2 TexCoord;
};

struct Vertex3D
{
    glm::vec3 position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Color;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
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
    vector<Texture> textures;
    VertexArray vertexArray;
    Primitive();
    Primitive(const std::string &path);
    void SetupTextures(vector<Texture> textures);
    void Draw(Shader &shader);

private:
    // render data
    bool is2D;
    int vertexCount;
    std::string filePath;
    void SetupData(const char *path);
    int GetVertexCount(char *fileData);
    void ProcessData(char *fileData);
    void SetupPrimitive();
};

#endif