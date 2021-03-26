#ifndef LIGHT_H
#define LIGHT_H

#include <rendering/Renderer.h>
#include <utility/CustomMath.h>

enum LIGHT_TYPE
{
    LIGHT_NORMAL,
    LIGHT_POINT,
    LIGHT_DIRECTION,
    LIGHT_SPOTLIGHT,
};

class Light
{
public:
    LIGHT_TYPE type;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Light();

private:
};

class NormalLight : public Light
{
public:
    glm::vec3 position;
    NormalLight();
    NormalLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                glm::vec3 position);

private:
};

class PointLight : public NormalLight
{
public:
    float constant;
    float linear;
    float quadratic;
    PointLight();
    PointLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
               glm::vec3 position, float constant = 1.0f, float linear = 0.22f, float quadratic = 0.2f);

private:
};

class DirectionalLight : public Light
{
public:
    glm::vec3 direction;
    DirectionalLight();
    DirectionalLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                     glm::vec3 lightDir);

private:
};

class SpotLight : public NormalLight
{
public:
    glm::vec3 direction;
    float cutoff;
    float outerCutoff;
    SpotLight();
    SpotLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
              glm::vec3 position, glm::vec3 lightDir, float lightCutoff, float lightOuterCutoff);

private:
};

glm::vec3 GetWorldPosition(glm::vec3 position, float angleVal, glm::vec3 scale);
#endif