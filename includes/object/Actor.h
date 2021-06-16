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
    CameraActor();

private:
    Camera cam;
};

// Template Actor
class RenderActor : public Actor
{
public:
    Material mat;
    std::string path;
    bool isVisible;
    RenderActor();
    RenderActor(std::string name_, ACTOR_TYPES type_, std::string path_ = "");

private:
};

class PrimitiveActor : public RenderActor
{
public:
    PrimitiveActor(std::string name_, std::string path_ = "");
};

class ModelActor : public RenderActor
{
public:
    ModelActor(std::string name_, std::string path_ = "");
};

class LightActor : public RenderActor
{
public:
    LightActor();
};

// Default Actors
enum TEMPLATE_ACTORS
{
    TRIANGLE_2D,
    RECTANGLE_2D,
    QUAD_2D,
    CUBE_3D,
    QUAD_3D,
    PLANE_3D,
    CUBEMAP_3D,
    BACKPACK_MODEL,
    SPHERE_MODEL,
    NORMAL_LIGHT_ACTOR,
    POINT_LIGHT_ACTOR,
    DIR_LIGHT_ACTOR,
    SPT_LIGHT_ACTOR,
    CAMERA_OBJECT_ACTOR,
};

extern ACTOR_TYPES actor_types[14];

extern std::string resource_dir;

extern std::string template_actor_filepath[14];

extern SHADER_TYPE default_actor_shaders[14];

extern unsigned int default_actor_id[14];
#endif // !ACTOR_H