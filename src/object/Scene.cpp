#include <object/Scene.h>

SceneData::SceneData(std::string sceneName_)
{
    sceneName = sceneName_;
    backgroundColor = Vec4(DEFAULT_SCENE_COLOR, 1.0f);
}

void SceneData::AddPrimitive(std::string path, int id, int actor_id)
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

void SceneData::AddShader(std::string path1, std::string path2, int id, int actor_id)
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

void SceneData::AddTexture(int id, int actor_id)
{
    if (!textures.is_id_present(id))
    {
        Log("Loading new texture");
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

void SceneData::AddModel(std::string path, int id, int actor_id)
{
}

void SceneData::AddLight()
{
}

void SceneData::DrawActor(RenderActor *actor, int actor_id, CameraActor *cam, Vec2 screen_dimension)
{
    Mat4 view = cam->GetCamera()->GetViewMatrix();
    Mat4 projection(1.0f);
    if (cam->isOrtho)
    {
        float aspectRatio = ((float)screen_dimension.x) / screen_dimension.y;
        float orthoSize = cam->camSize;
        Vec2 camDimension(aspectRatio * orthoSize, orthoSize);
        projection = glm::ortho(-camDimension.x / 2.0f, camDimension.x / 2.0f, -camDimension.y / 2.0f, camDimension.y / 2.0f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
    }
    else
    {
        projection = glm::perspective(glm::radians(cam->GetCamera()->Zoom), ((float)screen_dimension.x) / ((float)screen_dimension.y), CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
    }
    Vec3 actorPos(actor->transform.position);
    Vec3 actorRot(actor->transform.rotation);
    Vec3 actorScale(actor->transform.scale);
    Mat4 model(1.0f);
    model = glm::translate(model, actorPos);
    model = glm::rotate(model, glm::radians(actorRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(actorRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(actorRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, actorScale);

    switch (actor->type)
    {
    case PRIMITIVE_ACTOR:
        shaders.get_data_point_from_actor(actor_id)->data.use();
        shaders.get_data_point_from_actor(actor_id)->data.setVec3("matColor", actor->mat.albedo.col);
        shaders.get_data_point_from_actor(actor_id)->data.SetMatrices(model, view, projection);
        if (textures.get_data_count_from_actor(actor_id) > 0)
        {
            std::vector<Texture> prmTexs;
            std::vector<ActorData<Texture> *> datalist = textures.get_data_points_from_actor(actor_id);
            for (int i = 0; i < textures.get_data_count_from_actor(actor_id); i++)
            {
                prmTexs.push_back(datalist[i]->data);
            }
            prms.get_data_point_from_actor(actor_id)->data.SetupTextures(prmTexs);
        }
        prms.get_data_point_from_actor(actor_id)->data.Draw(shaders.get_data_point_from_actor(actor_id)->data);
        break;

    default:
        break;
    }
}

void SceneData::RemoveActor(int actor_id, DataTypes type)
{
    switch (type)
    {
    case SHADER_DATA:
        shaders.remove_actor_from_list(actor_id);
        break;
    case TEXTURE_DATA:
        textures.remove_actor_from_list(actor_id);
        break;
    default:
        break;
    }
}

Scene::Scene(std::string name)
{
    data = SceneData(name);
    actorCount = 0;
    camCount = 0;
    AddActor(CAMERA_OBJECT_ACTOR);
}

void Scene::AddActor(TEMPLATE_ACTORS actor_choice)
{
    if (actor_types[actor_choice] == CAMERA_ACTOR)
    {
        camCount++;
        CameraActor newActor("Main Camera", camCount);
        cameraList.push_back(newActor);
    }
    else
    {
        actorCount++;
        std::string actor_file_path = resource_dir + template_actor_filepath[actor_choice];
        RenderActor newActor("Object" + std::to_string(actorCount), actor_choice, actorCount);
        actorList.push_back(newActor);
        switch (newActor.type)
        {
        case PRIMITIVE_ACTOR:
            data.AddShader(GetVSPath(newActor.mat.type), GetFSPath(newActor.mat.type), static_cast<int>(newActor.mat.type), actorCount);
            data.AddPrimitive(actor_file_path, static_cast<int>(actor_choice), actorCount);
            break;

        default:
            break;
        }
    }
}

void Scene::UpdateActor(RenderActor *actor)
{
    if ((actor->mat.albedo.texID - 1) >= 0)
    {
        data.RemoveActor(actor->id, SHADER_DATA);
        data.RemoveActor(actor->id, TEXTURE_DATA);
        actor->mat.albedo.tex = GetTexTemplate((actor->mat.albedo.texID - 1));
        actor->mat.albedo.col = COLOR_WHITE;
        actor->mat.type = ((actor->mat.type == COLOR_2D) ? TEXTURE_2D : ((actor->mat.type == COLOR_3D) ? TEXTURE_3D : actor->mat.type));
        data.AddShader(GetVSPath(actor->mat.type), GetFSPath(actor->mat.type), static_cast<int>(actor->mat.type), actor->id);
        data.AddTexture(actor->mat.albedo.texID - 1, actor->id);
    }
    else if (actor->mat.albedo.texID == 0)
    {
        data.RemoveActor(actor->id, SHADER_DATA);
        data.RemoveActor(actor->id, TEXTURE_DATA);
        actor->mat.albedo.col = DEFAULT_SHADER_COLOR;
        actor->mat.type = ((actor->mat.type == TEXTURE_2D) ? COLOR_2D : ((actor->mat.type == TEXTURE_3D) ? COLOR_3D : actor->mat.type));
        data.AddShader(GetVSPath(actor->mat.type), GetFSPath(actor->mat.type), static_cast<int>(actor->mat.type), actor->id);
    }
}

void Scene::DrawScene(Renderer &renderer)
{
    // Get Data
    int width = (int)renderer.GetCurrentWidth();
    int height = (int)renderer.GetCurrentHeight();
    // Refresh Last Frame
    renderer.SetColor(data.backgroundColor.x, data.backgroundColor.y, data.backgroundColor.z, 1.0f);
    // renderer.frameBuffer.NewFrame(width, height);
    // Refresh Camera
    renderer.SetCamera(cameraList[0].GetCamera());
    renderer.ProcessInput(cameraList[0].canMove);
    // Render Objects
    for (int i = 0; i < actorCount; i++)
    {
        if (actorList[i].isVisible)
        {
            data.DrawActor(&actorList[i], actorList[i].id, &cameraList[0], Vec2(width, height));
        }
    }
    // Render FBO
    // renderer.frameBuffer.UnBindFBO();
}
