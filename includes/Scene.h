#ifndef SCENE_H
#define SCENE_H

#include <CustomMath.h>
#include <Entity.h>

#include <iostream>
#include <vector>

class Scene
{
public:
    glm::vec4 backgroundColor;
    Scene(std::string name);
    Scene();
    void AddEntity();
    void DrawScene();

private:
    std::string sceneName;
};

#endif