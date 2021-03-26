#ifndef MODEL_H
#define MODEL_H

#include <external/glad/glad.h>

#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>

#include <external/stb_image.h>

#include <external/assimp/Importer.hpp>
#include <external/assimp/scene.h>
#include <external/assimp/postprocess.h>

#include <rendering/Shader.h>
#include <object/Mesh.h>
#include <rendering/Texture.h>

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
    void FreeData();

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