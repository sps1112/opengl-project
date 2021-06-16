#include <object/Actor.h>

Actor::Actor(std::string name_, Transform transform_)
    : name(name_), transform(transform_) {}

CameraActor::CameraActor()
{
}

RenderActor::RenderActor()
{
    isVisible = true;
    mat = Material(defaultMat2D);
}

RenderActor::RenderActor(std::string name_, ACTOR_TYPES type_, std::string path_)
{
    name = name_;
    transform = Transform();
    type = type_;
    path = path_;
    isVisible = true;
    mat = Material(DEFAULT_SHADER_COLOR, COLOR_BLACK, default_actor_shaders[type]);
}

PrimitiveActor::PrimitiveActor(std::string name_, std::string path_)
{
    name = name_;
    transform = Transform();
    type = PRIMITIVE_ACTOR;
    path = path_;
}

ModelActor::ModelActor(std::string name_, std::string path_)
{
    name = name_;
    transform = Transform();
    type = MODEL_ACTOR;
    path = path_;
}

LightActor::LightActor()
{
}

ACTOR_TYPES actor_types[] = {
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    PRIMITIVE_ACTOR,
    MODEL_ACTOR,
    MODEL_ACTOR,
    LIGHT_ACTOR,
    LIGHT_ACTOR,
    LIGHT_ACTOR,
    LIGHT_ACTOR,
    CAMERA_ACTOR,
};

std::string resource_dir = "resources/";

std::string template_actor_filepath[] = {
    "primitives/2D/triangle.2d",
    "primitives/2D/rectangle.2d",
    "primitives/2D/quad.2d",
    "primitives/3D/cube.3d",
    "primitives/3D/quad.3d",
    "primitives/3D/plane.3d",
    "primitives/3D/cubemap.3d",
    "models/backpack/backpack.obj",
    "models/sphere/sphere.obj",
    " ",
    " ",
    " ",
    " ",
    " "};

SHADER_TYPE default_actor_shaders[] = {
    COLOR_2D,
    COLOR_2D,
    COLOR_2D,
    COLOR_3D,
    COLOR_3D,
    COLOR_3D,
    COLOR_3D,
    COLOR_3D,
    COLOR_3D,
    LIGHT_SHADER,
    LIGHT_SHADER,
    LIGHT_SHADER,
    LIGHT_SHADER,
    COLOR_2D};

unsigned int default_actor_id[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13};