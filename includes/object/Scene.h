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
    int id;
    std::vector<int> actor_ids;
    int actor_count = 0;
    ActorData(T data_, int id_, int actor_id_)
    {
        data = data_;
        id = id_;
        add_actor(actor_id_);
    }

    void add_actor(int actor_id)
    {
        actor_ids.push_back(actor_id);
        actor_count++;
    }

    int get_index_of_actor(int actor_id)
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

    void remove_at_index(int index)
    {
        std::vector<int> new_actor_ids;
        for (int i = 0; i < actor_count; i++)
        {
            if (i != index)
            {
                new_actor_ids.push_back(actor_ids[i]);
            }
        }
        actor_ids.clear();
        for (int i = 0; i < actor_count - 1; i++)
        {
            if (i != index)
            {
                actor_ids.push_back(new_actor_ids[i]);
            }
        }
    }

    void remove_actor(int actor_id)
    {
        if (has_actor(actor_id))
        {
            int index = get_index_of_actor(actor_id);
            actor_ids.erase(actor_ids.begin() + index);
            // remove_at_index(index);
            actor_count--;
        }
    }

    bool has_actor(int actor_id)
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

    ActorData<T> *get_data_point(int id)
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
        return NULL;
    }

    ActorData<T> *get_data_point_from_actor(int actor_id)
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
        return NULL;
    }

    int get_data_count_from_actor(int actor_id)
    {
        if (count > 0)
        {
            int totalCount = 0;
            for (int i = 0; i < count; i++)
            {
                if (list[i].has_actor(actor_id))
                {
                    totalCount++;
                }
            }
            return totalCount;
        }
        return 0;
    }

    std::vector<ActorData<T> *> get_data_points_from_actor(int actor_id)
    {
        std::vector<ActorData<T> *> datalist;
        for (int i = 0; i < count; i++)
        {
            if (list[i].has_actor(actor_id))
            {
                datalist.push_back(&list[i]);
            }
        }
        return datalist;
    }

    bool is_id_present(int test_id)
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

    bool is_actor_present(int actor_id)
    {
        for (int i = 0; i < count; i++)
        {
            if (list[i].has_actor(actor_id))
            {
                return true;
            }
        }
        return false;
    }

    void remove_actor_from_list(int actor_id)
    {
        if (is_actor_present(actor_id))
        {
            for (int i = 0; i < count; i++)
            {
                if (list[i].has_actor(actor_id))
                {
                    list[i].remove_actor(actor_id);
                }
            }
        }
    }

private:
};

enum DataTypes
{
    PRIMITIVE_DATA,
    SHADER_DATA,
    TEXTURE_DATA,
    MODEL_DATA,
    LIGHT_DATA,
};

// Scene Data class holding unique reference
class SceneData
{
public:
    glm::vec4 backgroundColor;
    std::string sceneName;
    SceneData(std::string sceneName_ = "New Scene");
    void AddPrimitive(std::string path, int id, int actor_id);
    void AddShader(std::string path1, std::string path2, int id, int actor_id);
    void AddTexture(int id, int actor_id);
    void AddModel(std::string path, int id, int actor_id);
    void AddLight();
    void DrawActor(RenderActor *actor, int actor_id, Camera *cam, Vec2 screen_dimension);
    void RemoveActor(int actor_id, DataTypes type);

private:
    ActorDataList<Primitive> prms;
    ActorDataList<Shader> shaders;
    ActorDataList<Texture> textures;
    ActorDataList<Model> models;
    ActorDataList<Light> lights;
};

class Scene
{
public:
    SceneData data;
    int actorCount;
    std::vector<RenderActor> actorList;
    int camCount;
    std::vector<CameraActor> cameraList;
    Scene(std::string name = "New Scene");
    void AddActor(TEMPLATE_ACTORS actor_choice);
    void UpdateActor(RenderActor *actor);
    void DrawScene(Renderer &renderer);

private:
    // Functions
    bool CheckList(TEMPLATE_ACTORS actor_choice);
};

#endif