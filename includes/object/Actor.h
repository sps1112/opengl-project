#ifndef ACTOR_H
#define ACTOR_H

// Header declarations
#include <object/Transform.h>
#include <rendering/Camera.h>
#include <rendering/Material.h>
#include <object/Primitive.h>
#include <object/Model.h>
#include <rendering/Light.h>

#include <iostream>

// Type of Actors
enum ACTOR_TYPES
{
    PRIMITIVE_ACTOR,
    MODEL_ACTOR,
    LIGHT_ACTOR,
    CAMERA_ACTOR,
};

// Actor Class
class Actor
{
public:
    std::string name;
    Transform transform;
    ACTOR_TYPES type;
    // Class Constructor
    Actor(std::string name_ = "New Actor", Transform transform_ = Transform());

private:
};

// Template Camera Actor
class CameraActor : public Actor
{
public:
private:
    Camera cam;
};

// Template Actor
class RenderActor : public Actor
{
public:
    Material mat;
    void Draw();

private:
};

class PrimitiveActor : public RenderActor
{
public:
private:
    Primitive prm;
};

class ModelActor : public RenderActor
{
public:
private:
    Model mod;
};

class LightActor : public RenderActor
{
public:
private:
    Light light;
};

#endif // !ACTOR_H