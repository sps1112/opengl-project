#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Shader.h>
#include <Mesh.h>

#include <string>
#include <vector>
using namespace std;

class Model
{
public:
    Model(char *path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader);
        }
    }

private:
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path)
    {
        Assimp::Importer importer;
    }
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif