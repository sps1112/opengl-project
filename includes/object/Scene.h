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
    std::vector<unsigned int> actor_ids;
    int actor_count = 0;
    ActorData(T data_, unsigned int id_, unsigned int actor_id_)
    {
        data = data_;
        id = id_;
        add_actor(actor_id_);
    }

    void add_actor(unsigned int actor_id)
    {
        Log("add new actor");
        actor_ids.push_back(actor_id);
        actor_count++;
        Log(actor_count);
        Log(actor_id);
    }

    int get_index_of_actor(unsigned int actor_id)
    {
        for (int i = 0; i < actor_count; i++)
        {
            if (actor_ids[i] == actor_id)
            {
                return i;
            }
        }
        return -1;
    }

    void remove_actor(unsigned int actor_id)
    {
        if (has_actor(actor_id))
        {
            int index = get_index_of_actor(actor_id);
            actor_ids.erase(actor_ids.begin() + index);
            actor_count--;
        }
    }

    bool has_actor(unsigned int actor_id)
    {
        for (int i = 0; i < actor_count; i++)
        {
            if (actor_ids[i] == actor_id)
            {
                return true;
            }
        }
        return false;
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

    ActorData<T> *get_data_point(unsigned int id)
    {
        if (count > 0)
        {
            for (int i = 0; i < count; i++)
            {
                if (list[i].id == id)
                {
                    return &(list[i]);
                }
            }
        }
    }

    ActorData<T> *get_data_point_from_actor(unsigned int actor_id)
    {
        if (count > 0)
        {
            for (int i = 0; i < count; i++)
            {
                if (list[i].has_actor(actor_id))
                {
                    return &(list[i]);
                }
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
    void AddPrimitive(std::string path, unsigned int id, unsigned int actor_id);
    void AddShader(std::string path1, std::string path2, unsigned int id, unsigned int actor_id);
    void AddTexture(unsigned int id, unsigned int actor_id);
    void AddModel(std::string path, unsigned int id, unsigned int actor_id);
    void AddLight();
    void DrawObject(RenderActor *actor, unsigned int actor_id);

private:
    ActorDataList<Primitive> prms;
    ActorDataList<Shader> shaders;
    ActorDataList<Texture> textures;
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