#include <rendering/Light.h>

Light::Light()
{
    ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(0.6);
}

NormalLight::NormalLight()
{
    ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(0.6);
}

NormalLight::NormalLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                         glm::vec3 position)
{
    type = LIGHT_NORMAL;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->position = position;
}

PointLight::PointLight()
{
    ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(0.6);
}

PointLight::PointLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                       glm::vec3 position, float constant, float linear, float quadratic)
{
    type = LIGHT_POINT;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

DirectionalLight::DirectionalLight()
{
    ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(0.6);
}

DirectionalLight::DirectionalLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                                   glm::vec3 lightDir)
{
    type = LIGHT_DIRECTION;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->direction = lightDir;
}

SpotLight::SpotLight()
{
    ambient = glm::vec3(0.2f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(0.6);
}

SpotLight::SpotLight(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular,
                     glm::vec3 position, glm::vec3 lightDir, float lightCutoff, float lightOuterCutoff)
{
    type = LIGHT_SPOTLIGHT;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->position = position;
    this->direction = lightDir;
    cutoff = lightCutoff;
    outerCutoff = lightOuterCutoff;
}

glm::vec3 GetWorldPosition(glm::vec3 position, float angleVal, glm::vec3 scale)
{
    glm::mat4 lightModel(1.0f);
    lightModel = glm::rotate(lightModel, glm::radians(angleVal), glm::vec3(0.0f, 1.0f, 0.0f));
    lightModel = glm::translate(lightModel, position);
    lightModel = glm::scale(lightModel, scale);
    return (lightModel * glm::vec4(1.0f));
}