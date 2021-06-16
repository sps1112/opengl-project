#include <object/Scene.h>

SceneData::SceneData(std::string sceneName_)
{
    sceneName = sceneName_;
    backgroundColor = Vec4(DEFAULT_SCENE_COLOR, 1.0f);
}

void SceneData::AddPrimitive(std::string path, unsigned int id)
{
    Log("Add Primitive");
    if (!prms.is_id_present(id))
    {
        Log("Loading New Primitive");
        Primitive prm(FileSystem::getPath(path));
        ActorData datapoint(prm, id);
        prms.add_data(datapoint);
    }
    else
    {
        Log("Primitive already present");
    }
}

void SceneData::AddShader(std::string path1, std::string path2, unsigned int id)
{
    if (!shaders.is_id_present(id))
    {
        Shader shd(FileSystem::getPath(path1), FileSystem::getPath(path2));
        ActorData datapoint(shd, id);
        shaders.add_data(datapoint);
    }
}

void SceneData::AddTexture(std::string path, unsigned int id)
{
}

void SceneData::AddModel(std::string path, unsigned int id)
{
}

void SceneData::AddLight()
{
}

void SceneData::DrawObject(RenderActor &actor)
{
    unsigned int id = default_actor_id[actor.type];
    switch (actor.type)
    {
    case PRIMITIVE_ACTOR:
        shaders.get_data_point(id).data.use();
        shaders.get_data_point(id).data.setVec3("matColor", actor.mat.albedo);
        prms.get_data_point(id).data.Draw(shaders.get_data_point(id).data);
        break;

    default:
        break;
    }
}

Scene::Scene(std::string name)
{
    data = SceneData(name);
    actorCount = 0;
}

void Scene::AddObject(TEMPLATE_ACTORS actor_choice)
{
    actorCount++;
    std::string actor_file_path = resource_dir + template_actor_filepath[actor_choice];
    RenderActor newActor("Object" + std::to_string(actorCount), actor_types[actor_choice], actor_file_path);
    actorList.push_back(newActor);
    switch (newActor.type)
    {
    case PRIMITIVE_ACTOR:
        data.AddShader(GetVSPath(newActor.mat.type), GetFSPath(newActor.mat.type), default_actor_id[actor_choice]);
        data.AddPrimitive(actor_file_path, default_actor_id[actor_choice]);
        break;

    default:
        break;
    }
}

void Scene::DrawScene(Renderer &renderer)
{
    // Get Data
    /*int width = (int)renderer.GetCurrentWidth();
    int height = (int)renderer.GetCurrentHeight();*/
    // Refresh Last Frame
    renderer.SetColor(data.backgroundColor.x, data.backgroundColor.y, data.backgroundColor.z, 1.0f);
    // renderer.frameBuffer.NewFrame(width, height);
    // Render Objects
    for (int i = 0; i < actorCount; i++)
    {
        if (actorList[i].isVisible)
        {
            data.DrawObject(actorList[i]);
        }
    }
    // Render FBO
    // renderer.frameBuffer.UnBindFBO();
}