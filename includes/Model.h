#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Shader.h>
#include <Mesh.h>
#include <Texture.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class Model
{
public:
    // model data
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    Model(string const &path, bool gamma = false);
    void Draw(Shader &shader);

private:
    // loads scene
    void loadModel(string const &path);
    // processes a node of the scene
    void processNode(aiNode *node, const aiScene *scene);
    // converts assimp data to Mesh Class
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    // load texture based on type
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
#endif