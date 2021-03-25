#ifndef SCENE_H
#define SCENE_H

#include <CustomMath.h>
#include <Renderer.h>
#include <FileSystem.h>
#include <Primitive.h>
#include <Utils.h>
#include <Shader.h>
#include <Model.h>
#include <Texture.h>

#include <iostream>
#include <vector>

enum OBJECT_TYPE
{
    PRIMITIVE_OBJECT,
    LIGHT_OBJECT,
    MODEL_OBJECT,
};

class SceneObject
{
public:
    std::string name;
    std::string path;
    OBJECT_TYPE objectType;
    bool isVisible;
    Shader shader;
    std::vector<Texture> textures;
    SceneObject();
    SceneObject(std::string name, std::string path, OBJECT_TYPE objectType);
    void AddShader(std::string pathVertex, std::string pathFragment);
    void AddTexture(TEXTURE_TYPE type, std::string path,
                    bool gammaCorrection = false, bool isDiffuse = true, bool toClamp = false);

private:
    Transform transform;
};

class UniqueObject : public SceneObject
{
public:
    int count;
    UniqueObject(std::string name, std::string path, OBJECT_TYPE objectType);
    void Draw(SceneObject *object);

private:
    Primitive primitive;
};

class Scene
{
public:
    glm::vec4 backgroundColor;
    std::string sceneName;
    int objectCount;
    std::vector<SceneObject> objects;
    int uniqueCount;
    std::vector<UniqueObject> uniques;
    Scene(std::string name);
    Scene();
    void AddObject(const std::string &path, OBJECT_TYPE type);
    void DrawScene(Renderer &renderer);
    UniqueObject *GetUnique(std::string path);

private:
    // Functions
    bool CheckList(std::string path);
};

#endif