#include <Scene.h>

Scene::Scene(std::string name)
{
    sceneName = name;
    backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

Scene::Scene()
{
    backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Scene::AddEntity()
{
}

void Scene::DrawScene()
{
}