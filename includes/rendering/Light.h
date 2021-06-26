#ifndef LIGHT_H
#define LIGHT_H

// Header declarations
#include <utility/CustomMath.h>
#include <rendering/Material.h>
#include <object/Transform.h>
#include <config.h>

// Types of Light sources
enum LIGHT_TYPE
{
    LIGHT_COLOR,
    LIGHT_BASE,
    LIGHT_NORMAL,
    LIGHT_POINT,
    LIGHT_DIRECTION,
    LIGHT_SPOTLIGHT,
};

// A simple light with a Color Value
class Light
{
public:
    ColorF diffuse;  // Color of the Light Source
    LIGHT_TYPE type; // Type of Light Source
    // Light Class Constructor
    Light(ColorF lightColor = DEFAULT_LIGHT_COLOR);

private:
};

// A Light Source for the Phong Model
class BaseLight : public Light
{
public:
    ColorF ambient;  // Ambient Lighting Color
    ColorF specular; // Specular Lighting Color
    // Base Light Class Constructor
    BaseLight(ColorF lightColor = DEFAULT_LIGHT_COLOR, float diffuseFactor = 0.6f);

private:
};

// A Normal Light Source positioned at a Point in Space
class NormalLight : public BaseLight
{
public:
    Vec3 position; // Position of Light Source
    // Normal Light Class Constructor
    NormalLight(Vec3 position_ = WORLD_ORIGIN, ColorF lightColor = DEFAULT_LIGHT_COLOR, float diffuseFactor = 0.6f);

private:
};

// A Directional Light Source with parallel light rays coming from a direction
class DirectionalLight : public BaseLight
{
public:
    Vec3 direction; // Direction of the Light Rays
    // Directional Light Class Constructor
    DirectionalLight(Vec3 direction_ = WORLD_LEFT_DIAGONAL, ColorF lightColor = DEFAULT_LIGHT_COLOR, float diffuseFactor = 0.6f);

private:
};

// A Point Light Source with Attenuation
class PointLight : public NormalLight
{
public:
    float constant;  // Attenuation Constant Term
    float linear;    // Attenuation Linear Term
    float quadratic; // Attenuation Quadratic Term
    // Point Light Class Constructor
    PointLight(Vec3 position_ = WORLD_ORIGIN, ColorF lightColor = DEFAULT_LIGHT_COLOR, float diffuseFactor = 0.6f,
               float constant_ = 1.0f, float linear_ = 0.22f, float quadratic_ = 0.2f);

private:
};

// A Spot Light with cutoff angles
class SpotLight : public NormalLight
{
public:
    Vec3 direction;    // Direction of the Spot Light
    float cutoff;      // Inner cutoff angle
    float outerCutoff; // Outer cutoff angle
    // Spot Light Class Constructor
    SpotLight(Vec3 position_ = CAMERA_ORIGIN, ColorF lightColor = DEFAULT_LIGHT_COLOR, Vec3 direction_ = -WORLD_FORWARD, float diffuseFactor = 0.6f,
              float cutoff_ = 15.0f, float outerCutoff_ = 22.5f);

private:
};

// Returns the World Position of the Light Source
Vec3 get_world_pos(Transform *transform);

#endif // !LIGHT_H
