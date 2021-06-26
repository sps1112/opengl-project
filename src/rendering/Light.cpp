#include <rendering/Light.h>

Light::Light(ColorF lightColor)
{
    type = LIGHT_COLOR;
    diffuse = lightColor;
}

BaseLight::BaseLight(ColorF lightColor, float diffuseFactor)
{
    type = LIGHT_BASE;
    ambient = lightColor * 0.1f;
    diffuse = lightColor * diffuseFactor;
    specular = DEFAULT_LIGHT_COLOR * 0.5f;
}

NormalLight::NormalLight(Vec3 position_, ColorF lightColor, float diffuseFactor)
{
    type = LIGHT_NORMAL;
    position = position_;
    ambient = lightColor * 0.1f;
    diffuse = lightColor * diffuseFactor;
    specular = DEFAULT_LIGHT_COLOR * 0.5f;
}

DirectionalLight::DirectionalLight(Vec3 direction_, ColorF lightColor, float diffuseFactor)
{
    type = LIGHT_DIRECTION;
    direction = direction_;
    ambient = lightColor * 0.1f;
    diffuse = lightColor * diffuseFactor;
    specular = DEFAULT_LIGHT_COLOR * 0.5f;
}

PointLight::PointLight(Vec3 position_, ColorF lightColor, float diffuseFactor,
                       float constant_, float linear_, float quadratic_)
{
    type = LIGHT_POINT;
    position = position_;
    ambient = lightColor * 0.1f;
    diffuse = lightColor * diffuseFactor;
    specular = DEFAULT_LIGHT_COLOR * 0.5f;
    constant = constant_;
    linear = linear_;
    quadratic = quadratic_;
}

SpotLight::SpotLight(Vec3 position_, ColorF lightColor, Vec3 direction_, float diffuseFactor,
                     float cutoff_, float outerCutoff_)
{
    type = LIGHT_SPOTLIGHT;
    position = position_;
    ambient = lightColor * 0.1f;
    diffuse = lightColor * diffuseFactor;
    specular = DEFAULT_LIGHT_COLOR * 0.5f;
    direction = direction_;
    cutoff = cutoff_;
    outerCutoff = outerCutoff_;
}

Vec3 get_world_pos(Transform *transform)
{
    Mat4 lightModel(1.0f);
    lightModel = glm::rotate(lightModel, glm::radians(transform->rotation.y), Vec3(0.0f, 1.0f, 0.0f));
    lightModel = glm::translate(lightModel, transform->position);
    lightModel = glm::scale(lightModel, transform->scale);
    return (lightModel * Vec4(1.0f));
}
