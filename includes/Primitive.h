#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <FileReader.h>
#include <Utils.h>
#include <Renderer.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

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
    VertexArray vertexArray;
    Primitive(const char *path);
    void SetupTextures(vector<pTexture> textures);
    void Draw(Shader &shader);

private:
    // render data
    bool is2D;
    int vertexCount;
    void SetupData(const char *path);
    int GetVertexCount(char *fileData);
    void ProcessData(char *fileData);
    void SetupPrimitive();
};

#endif