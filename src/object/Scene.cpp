#include <object/Scene.h>

Scene::Scene(std::string name)
{
    sceneName = name;
    backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    objectCount = 0;
    uniqueCount = 0;
}

Scene::Scene()
{
    backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    objectCount = 0;
    uniqueCount = 0;
}

void Scene::AddObject(const std::string &path, OBJECT_TYPE type)
{
    objectCount++;
    SceneObject newObject("Object" + std::to_string(objectCount), path, type);
    newObject.AddShader("shaders/modern/shader_2d.vs", "shaders/modern/shader_2d.fs");
    newObject.AddTexture(TEXTURE_DIFFUSE, "resources/textures/awesomeface.png");
    objects.push_back(newObject);
    if (CheckList(path))
    {
        GetUnique(path)->count++;
    }
    else
    {
        uniqueCount++;
        UniqueObject newUnique("Unique" + std::to_string(uniqueCount), path, type);
        uniques.push_back(newUnique);
    }
}

void Scene::DrawScene(Renderer &renderer)
{
    // Get Data
    /*int width = (int)renderer.GetCurrentWidth();
    int height = (int)renderer.GetCurrentHeight();*/
    // Refresh Last Frame
    renderer.SetColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
    // renderer.frameBuffer.NewFrame(width, height);
    // Render Objects
    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i].isVisible)
        {
            GetUnique(objects[i].path)->Draw(&objects[i]);
        }
    }
    // Render FBO
    // renderer.frameBuffer.UnBindFBO();
}

bool Scene::CheckList(std::string path)
{
    bool objectLoaded = false;
    for (int i = 0; i < uniqueCount; i++)
    {
        if (uniques[i].path == path)
        {
            objectLoaded = true;
            break;
        }
    }
    return objectLoaded;
}

UniqueObject *Scene::GetUnique(std::string path)
{
    int index = 0;
    for (int i = 0; i < uniqueCount; i++)
    {
        if (uniques[i].path == path)
        {
            index = i;
            break;
        }
    }
    return &(uniques[index]);
}

SceneObject::SceneObject()
{
    isVisible = true;
}

SceneObject::SceneObject(std::string name, std::string path, OBJECT_TYPE objectType)
{
    this->name = name;
    this->path = path;
    this->objectType = objectType;
    isVisible = true;
}

void SceneObject::AddShader(std::string pathVertex, std::string pathFragment)
{
    shader = Shader(FileSystem::getPath(pathVertex),
                    FileSystem::getPath(pathFragment));
}

void SceneObject::AddTexture(TEXTURE_TYPE type, std::string path,
                             bool gammaCorrection, bool isDiffuse, bool toClamp)
{
    Texture newTexture = LoadTexture(type, FileSystem::getPath(path), gammaCorrection, isDiffuse, toClamp);
    textures.push_back(newTexture);
}

UniqueObject::UniqueObject(std::string name, std::string path, OBJECT_TYPE objectType)
{
    this->name = name;
    this->path = path;
    this->objectType = objectType;
    this->count = 1;
    switch (objectType)
    {
    case PRIMITIVE_OBJECT:
        primitive = Primitive(FileSystem::getPath(path));
        break;
    }
}

void UniqueObject::Draw(SceneObject *object)
{
    switch (objectType)
    {
    case PRIMITIVE_OBJECT:
        primitive.SetupTextures(object->textures);
        (object->shader).use();
        primitive.Draw(object->shader);
        break;
    }
}