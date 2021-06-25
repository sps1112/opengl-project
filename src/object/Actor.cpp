#include <object/Actor.h>

Actor::Actor(std::string name_, Transform transform_)
    : name(name_), transform(transform_) {}

CameraActor::CameraActor(std::string name_, int id_, Transform transform_)
{
    name = name_;
    id = id_;
    cam = Camera(transform_.position);
    transform = Transform(cam.Position, Vec3(0.0f), Vec3(1.0f));
    canMove = false;
    canRotate = false;
    isOrtho = false;
    camSize = 5.0f;
}

Camera *CameraActor::GetCamera()
{
    return &cam;
}

void CameraActor::RefreshCamera(bool transformCallback)
{
    if (transformCallback)
    {
        cam.Position = transform.position;
        cam.Yaw = transform.rotation.y + CAMERA_YAW;
        cam.Pitch = clamp(transform.rotation.x, -CAMERA_MAX_PITCH, CAMERA_MAX_PITCH);
        cam.updateCameraVectors();
        cam.Right = transform.get_right();
        cam.Up = transform.get_up();
    }
    else
    {
        transform.position = cam.Position;
        transform.rotation.y = overflow(cam.Yaw - CAMERA_YAW, 360.0f);
        transform.rotation.x = cam.Pitch;
        transform.rotation.z = 0;
    }
}

void CameraActor::ResetTransform()
{
    transform.reset_to_origin();
    transform.position = CAMERA_ORIGIN;
    RefreshCamera(true);
}

RenderActor::RenderActor()
{
    isVisible = true;
    mat = material_template[0];
}

RenderActor::RenderActor(std::string name_, TEMPLATE_ACTORS template_, int id_)
{
    name = name_;
    transform = Transform();
    type = actor_types[template_];
    actor_template = template_;
    id = id_;
    path = resource_dir + template_actor_filepath[template_];
    isVisible = true;
    mat = material_template[default_actor_materials[template_]];
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
    MODEL_ACTOR,
    MODEL_ACTOR,
    MODEL_ACTOR,
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
    "models/cyborg/cyborg.obj",
    "models/nanosuit/nanosuit.obj",
    "models/planet/planet.obj",
    "models/rock/rock.obj",
    "models/sphere/sphere.obj",
    "models/teapot/teapot.obj",
    " ",
    " ",
    " ",
    " ",
    " "};

MATERIAL_TEMPLATES default_actor_materials[] = {
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    COLOR_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    MODEL_3D_MAT,
    LIGHT_MAT,
    LIGHT_MAT,
    LIGHT_MAT,
    LIGHT_MAT,
    COLOR_2D_MAT};
