#ifndef MESH_H
#define MESH_H

#include <external/glad/glad.h>

#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>

#include <rendering/Shader.h>
#include <rendering/Renderer.h>
#include <rendering/Texture.h>

#include <string>
#include <vector>
using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    VertexArray vertexArray;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader &shader);

private:
    // render data
    void setupMesh();
};
#endif