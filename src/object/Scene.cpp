#include <object/Scene.h>

SceneData::SceneData(std::string sceneName_)
{
    sceneName = sceneName_;
    backgroundColor = Vec4(DEFAULT_SCENE_COLOR, 1.0f);
}

void SceneData::AddPrimitive(std::string path, unsigned int id, unsigned int actor_id)
{
    if (!prms.is_id_present(id))
    {
        Log("Loading New Primitive");
        Primitive prm(FileSystem::getPath(path));
        ActorData datapoint(prm, id, actor_id);
        prms.add_data(datapoint);
    }
    else
    {
        Log("Primitive already present");
        prms.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddShader(std::string path1, std::string path2, unsigned int id, unsigned int actor_id)
{
    if (!shaders.is_id_present(id))
    {
        Log("Loading New shader");
        Shader shd(FileSystem::getPath(path1), FileSystem::getPath(path2));
        ActorData datapoint(shd, id, actor_id);
        shaders.add_data(datapoint);
    }
    else
    {
        Log("shader already loaded");
        shaders.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddTexture(unsigned int id, unsigned int actor_id)
{
    if (!textures.is_id_present(id))
    {
        Log("Loading new texture");
        Log(id);
        Texture tex = get_from_template(GetTexTemplate(id));
        ActorData datapoint(tex, id, actor_id);
        textures.add_data(datapoint);
    }
    else
    {
        Log("Texture already loaded");
        textures.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddModel(std::string path, unsigned int id, unsigned int actor_id)
{
}

void SceneData::AddLight()
{
}

void SceneData::DrawObject(RenderActor *actor, unsigned int actor_id)
{
    switch (actor->type)
    {
    case PRIMITIVE_ACTOR:
        if ((actor->mat.albedo.texID - 1) >= 0)
        {
            shaders.get_data_point_from_actor(actor_id)->remove_actor(actor_id);
            textures.get_data_point_from_actor(actor_id)->remove_actor(actor_id);
            actor->mat.albedo.tex = GetTexTemplate((actor->mat.albedo.texID - 1));
            actor->mat.type = TEXTURE_2D;
            AddShader(GetVSPath(actor->mat.type), GetFSPath(actor->mat.type), static_cast<unsigned int>(actor->mat.type), actor_id);
            AddTexture(actor->mat.albedo.texID - 1, actor_id);
        }
        else
        {
            // Log("No texture");
        }
        shaders.get_data_point_from_actor(actor_id)->data.use();
        shaders.get_data_point_from_actor(actor_id)->data.setVec3("matColor", actor->mat.albedo.col);
        prms.get_data_point_from_actor(actor_id)->data.Draw(shaders.get_data_point_from_actor(actor_id)->data);
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
    RenderActor newActor("Object" + std::to_string(actorCount), actor_choice);
    actorList.push_back(newActor);
    switch (newActor.type)
    {
    case PRIMITIVE_ACTOR:
        data.AddShader(GetVSPath(newActor.mat.type), GetFSPath(newActor.mat.type), static_cast<unsigned int>(newActor.mat.type), actorCount);
        data.AddPrimitive(actor_file_path, static_cast<unsigned int>(actor_choice), actorCount);
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
            data.DrawObject(&actorList[i], (i + 1));
        }
    }
    // Render FBO
    // renderer.frameBuffer.UnBindFBO();
}