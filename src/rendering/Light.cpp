#include <rendering/Light.h>

Light::Light()
{
    ambient = Vec3(0.2f);
    diffuse = Vec3(0.5f);
    specular = Vec3(0.6f);
}

NormalLight::NormalLight(Vec3 position, Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular)
{
    type = LIGHT_NORMAL;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->position = position;
}

PointLight::PointLight()
{
    ambient = Vec3(0.2f);
    diffuse = Vec3(0.5f);
    specular = Vec3(0.6);
}

PointLight::PointLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
                       Vec3 position, float constant, float linear, float quadratic)
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
    ambient = Vec3(0.2f);
    diffuse = Vec3(0.5f);
    specular = Vec3(0.6);
}

DirectionalLight::DirectionalLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
                                   Vec3 lightDir)
{
    type = LIGHT_DIRECTION;
    ambient = lightAmbient;
    diffuse = lightDiffuse;
    specular = lightSpecular;
    this->direction = lightDir;
}

SpotLight::SpotLight()
{
    ambient = Vec3(0.2f);
    diffuse = Vec3(0.5f);
    specular = Vec3(0.6);
}

SpotLight::SpotLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
                     Vec3 position, Vec3 lightDir, float lightCutoff, float lightOuterCutoff)
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

Vec3 GetWorldPosition(Vec3 position, float angleVal, Vec3 scale)
{
    Mat4 lightModel(1.0f);
    lightModel = glm::rotate(lightModel, glm::radians(angleVal), Vec3(0.0f, 1.0f, 0.0f));
    lightModel = glm::translate(lightModel, position);
    lightModel = glm::scale(lightModel, scale);
    return (lightModel * Vec4(1.0f));
}