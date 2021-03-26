#include <object/Mesh.h>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffusedNR = 1;
    unsigned int specularNR = 1;
    unsigned int normalNR = 1;
    unsigned int heightNR = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        SetActiveTexture(i);
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
        shader.setFloat(("material." + name + number).c_str(), i);
        BindTexture(textures[i].id);
    }
    UnBindTexture();

    // draw mesh
    vertexArray.DrawElements(indices.size());
}

void Mesh::setupMesh()
{
    vertexArray.GenerateBuffers();
    vertexArray.BindVAO();
    vertexArray.BindVBO(vertices.size(), sizeof(Vertex), &vertices[0]);
    vertexArray.BindEBO(indices.size(), &indices[0]);
    // vertex positions
    vertexArray.SetAttribArray(0, 3, sizeof(Vertex), (void *)0);
    // vertex normals
    vertexArray.SetAttribArray(1, 3, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex Texture coordinates
    vertexArray.SetAttribArray(2, 2, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // vertex Tangents
    vertexArray.SetAttribArray(3, 3, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // vertex Bitangents
    vertexArray.SetAttribArray(4, 3, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));
    vertexArray.UnBindVAO();
}