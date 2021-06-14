#ifndef LIGHT_H
#define LIGHT_H

// Header declarations
#include <utility/CustomMath.h>

// Types of Light sources
enum LIGHT_TYPE
{
    LIGHT_NORMAL,
    LIGHT_POINT,
    LIGHT_DIRECTION,
    LIGHT_SPOTLIGHT,
};

// Base Light Class
class Light
{
public:
    LIGHT_TYPE type;
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    Light();

private:
};

// Normal Point Light Source
class NormalLight : public Light
{
public:
    Vec3 position;
    NormalLight(Vec3 position = Vec3(0.0f),
                Vec3 lightAmbient = Vec3(0.2f),
                Vec3 lightDiffuse = Vec3(0.5f),
                Vec3 lightSpecular = Vec3(0.6f));

private:
};

// Point Light with Attenuation
class PointLight : public NormalLight
{
public:
    float constant;
    float linear;
    float quadratic;
    PointLight();
    PointLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
               Vec3 position, float constant = 1.0f, float linear = 0.22f, float quadratic = 0.2f);

private:
};

// Directional Light Source
class DirectionalLight : public Light
{
public:
    Vec3 direction;
    DirectionalLight();
    DirectionalLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
                     Vec3 lightDir);

private:
};

// Spot Light Source
class SpotLight : public NormalLight
{
public:
    Vec3 direction;
    float cutoff;
    float outerCutoff;
    SpotLight();
    SpotLight(Vec3 lightAmbient, Vec3 lightDiffuse, Vec3 lightSpecular,
              Vec3 position, Vec3 lightDir, float lightCutoff, float lightOuterCutoff);

private:
};

Vec3 GetWorldPosition(Vec3 position, float angleVal, Vec3 scale);

#endif // !LIGHT_H