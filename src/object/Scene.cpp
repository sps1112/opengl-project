#include <object/Scene.h>

char *drawComboItems[] = {
    "Draw Line", "Draw Point", "Draw Fill"};

SceneData::SceneData(std::string sceneName_)
{
    sceneName = sceneName_;
    backgroundColor = Vec4(DEFAULT_SCENE_COLOR, 1.0f);
    drawMode = 2;
}

void SceneData::AddPrimitive(std::string path, int id, int actor_id)
{
    if (!prms.is_id_present(id))
    {
        log_message("Loading New Primitive");
        Primitive prm(FileSystem::get_path(path));
        ActorData datapoint(prm, id, actor_id);
        prms.add_data(datapoint);
    }
    else
    {
        log_message("Primitive already present");
        prms.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddShader(std::string path1, std::string path2, int id, int actor_id)
{
    if (!shaders.is_id_present(id))
    {
        log_message("Loading New shader");
        Shader shd(FileSystem::get_path(path1), FileSystem::get_path(path2));
        ActorData datapoint(shd, id, actor_id);
        shaders.add_data(datapoint);
    }
    else
    {
        log_message("shader already loaded");
        shaders.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddTexture(int id, int actor_id)
{
    if (!textures.is_id_present(id))
    {
        log_message("Loading new texture");
        Texture tex = get_from_template(get_tex_template(id));
        ActorData datapoint(tex, id, actor_id);
        textures.add_data(datapoint);
    }
    else
    {
        log_message("Texture already loaded");
        textures.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddModel(std::string path, int id, int actor_id)
{
    if (!models.is_id_present(id))
    {
        log_message("Loading New Model");
        Model mod(FileSystem::get_path(path));
        ActorData datapoint(mod, id, actor_id);
        models.add_data(datapoint);
    }
    else
    {
        log_message("Model already present");
        models.get_data_point(id)->add_actor(actor_id);
    }
}

void SceneData::AddLight()
{
}

void SceneData::DrawActor(RenderActor *actor, int actor_id, CameraActor *cam, Vec2 screen_dimension)
{
    Mat4 view = cam->GetCamera()->get_view_matrix();
    Mat4 projection(1.0f);
    screen_dimension.x = floor(screen_dimension.x, 0.0f);
    screen_dimension.y = floor(screen_dimension.y, 0.0f);
    if (cam->isOrtho)
    {
        float aspectRatio = (((float)screen_dimension.x) / ((float)screen_dimension.y));
        float orthoSize = cam->camSize;
        Vec2 camDimension(aspectRatio * orthoSize, orthoSize);
        projection = glm::ortho(-camDimension.x / 2.0f, camDimension.x / 2.0f, -camDimension.y / 2.0f, camDimension.y / 2.0f, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
    }
    else
    {
        projection = glm::perspective(glm::radians(cam->GetCamera()->fovZoom), ((float)screen_dimension.x) / ((float)screen_dimension.y), CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
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

    shaders.get_data_point_from_actor(actor_id)->data.use();
    shaders.get_data_point_from_actor(actor_id)->data.setVec3("matColor", actor->mat.albedo.col);
    shaders.get_data_point_from_actor(actor_id)->data.SetMatrices(model, view, projection);
    switch (actor->type)
    {
    case PRIMITIVE_ACTOR:
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

    case MODEL_ACTOR:
        models.get_data_point_from_actor(actor_id)->data.Draw(shaders.get_data_point_from_actor(actor_id)->data);
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
    is_camera_set = false;
}

void Scene::AddActor(TEMPLATE_ACTORS actor_choice)
{
    if (actorTemplates[actor_choice].type == CAMERA_ACTOR)
    {
        camCount++;
        CameraActor newActor("Main Camera", camCount);
        cameraList.push_back(newActor);
    }
    else
    {
        actorCount++;
        RenderActor newActor("Object" + std::to_string(actorCount), actor_choice, actorCount);
        actorList.push_back(newActor);
        data.AddShader(templateShaders[newActor.mat.type].get_vs_path(), templateShaders[newActor.mat.type].get_fs_path(), static_cast<int>(newActor.mat.type), actorCount);
        switch (newActor.type)
        {
        case PRIMITIVE_ACTOR:
            data.AddPrimitive(newActor.path, static_cast<int>(actor_choice), actorCount);
            break;
        case MODEL_ACTOR:
            data.AddModel(newActor.path, static_cast<int>(actor_choice), actorCount);
            break;
        case LIGHT_ACTOR:

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
        actor->mat.albedo.col = COLOR_WHITE;
        actor->mat.type = ((actor->mat.type == COLOR_2D_SHADER) ? TEXTURE_2D_SHADER : ((actor->mat.type == COLOR_3D_SHADER) ? TEXTURE_3D_SHADER : actor->mat.type));
        data.AddShader(templateShaders[actor->mat.type].get_vs_path(), templateShaders[actor->mat.type].get_fs_path(), static_cast<int>(actor->mat.type), actor->id);
        data.AddTexture(actor->mat.albedo.texID - 1, actor->id);
    }
    else if (actor->mat.albedo.texID == 0)
    {
        data.RemoveActor(actor->id, SHADER_DATA);
        data.RemoveActor(actor->id, TEXTURE_DATA);
        actor->mat.albedo.col = DEFAULT_SHADER_COLOR;
        actor->mat.type = ((actor->mat.type == TEXTURE_2D_SHADER) ? COLOR_2D_SHADER : ((actor->mat.type == TEXTURE_3D_SHADER) ? COLOR_3D_SHADER : actor->mat.type));
        data.AddShader(templateShaders[actor->mat.type].get_vs_path(), templateShaders[actor->mat.type].get_fs_path(), static_cast<int>(actor->mat.type), actor->id);
    }
}

void Scene::DrawScene(Renderer &renderer)
{
    // Get New Data
    int width = (int)renderer.GetCurrentWidth();
    int height = (int)renderer.GetCurrentHeight();

    // Process Data
    if (!is_camera_set)
    {
        renderer.SetCamera(cameraList[0].GetCamera());
        is_camera_set = true;
    }
    if (cameraList[0].canMove || cameraList[0].canRotate)
    {
        cameraList[0].RefreshCamera();
    }
    if (renderer.CheckInput(KEY_SPACE))
    {
        cameraList[0].canMove = true;
        cameraList[0].canRotate = true;
    }
    if (renderer.CheckInput(KEY_LFT_ALT))
    {
        cameraList[0].canMove = false;
        cameraList[0].canRotate = false;
    }
    renderer.ProcessInput(cameraList[0].canMove);
    renderer.ProcessMouse(cameraList[0].canRotate);
    renderer.SetDraw(data.drawMode);

    // Refresh Last Frame
    renderer.SetColor(data.backgroundColor.x, data.backgroundColor.y, data.backgroundColor.z, 1.0f);
    // renderer.frameBuffer.NewFrame(width, height);

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
