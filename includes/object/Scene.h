#ifndef SCENE_H
#define SCENE_H

// Header Declarations
#include <object/Actor.h>
#include <object/Primitive.h>
#include <rendering/Shader.h>
#include <rendering/Texture.h>
#include <object/Model.h>
#include <rendering/Light.h>
#include <utility/FileSystem.h>
#include <utility/CustomMath.h>
#include <config.h>

#include <iostream>
#include <vector>

// Template Data Class
template <typename T>
class ActorData
{
public:
    T data;
    unsigned int id;
    ActorData(T data_, unsigned int id_)
    {
        data = data_;
        id = id_;
    }

private:
};

// Template List Class of a Data Class
template <typename T>
class ActorDataList
{
public:
    std::vector<ActorData<T>> list;
    int count;

    ActorDataList()
    {
        count = 0;
    }

    void add_data(ActorData<T> new_data)
    {
        list.push_back(new_data);
        count++;
    }

    ActorData<T> get_data_point(unsigned int id)
    {
        for (int i = 0; i < count; i++)
        {
            if (list[i].id == id)
            {
                return list[i];
            }
        }
    }

    bool is_id_present(unsigned int test_id)
    {
        for (int i = 0; i < count; i++)
        {
            if (list[i].id == test_id)
            {
                return true;
            }
        }
        return false;
    }

private:
};

// Scene Data class holding unique reference
class SceneData
{
public:
    glm::vec4 backgroundColor;
    std::string sceneName;
    SceneData(std::string sceneName_ = "New Scene");
    void AddPrimitive(std::string path, unsigned int id);
    void AddShader(std::string path1, std::string path2, unsigned int id);
    void AddTexture(std::string path, unsigned int id);
    void AddModel(std::string path, unsigned int id);
    void AddLight();
    void DrawObject(RenderActor *actor);

private:
    ActorDataList<Primitive> prms;
    ActorDataList<Shader> shaders;
    std::vector<ActorData<Texture>> textures;
    std::vector<ActorData<Model>> models;
    std::vector<ActorData<Light>> lights;
};

class Scene
{
public:
    SceneData data;
    int actorCount;
    std::vector<RenderActor> actorList;
    Scene(std::string name = "New Scene");
    void AddObject(TEMPLATE_ACTORS actor_choice);
    void DrawScene(Renderer &renderer);

private:
    // Functions
    bool CheckList(TEMPLATE_ACTORS actor_choice);
};

#endif