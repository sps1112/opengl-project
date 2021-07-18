#include <object/Actor.h>

std::string resource_dir = "resources/";

ActorTemplate actorTemplates[] = {
    ActorTemplate(TRIANGLE_2D, PRIMITIVE_ACTOR, "primitives/2D/triangle.2d", COLOR_2D_MAT),
    ActorTemplate(RECTANGLE_2D, PRIMITIVE_ACTOR, "primitives/2D/rectangle.2d", COLOR_2D_MAT),
    ActorTemplate(QUAD_2D, PRIMITIVE_ACTOR, "primitives/2D/quad.2d", COLOR_2D_MAT),
    ActorTemplate(CUBE_3D, PRIMITIVE_ACTOR, "primitives/3D/cube.3d", COLOR_3D_MAT),
    ActorTemplate(TRIANGLE_3D, PRIMITIVE_ACTOR, "primitives/2D/triangle.2d", COLOR_3D_MAT),
    ActorTemplate(QUAD_3D, PRIMITIVE_ACTOR, "primitives/3D/quad.3d", COLOR_3D_MAT),
    ActorTemplate(PLANE_3D, PRIMITIVE_ACTOR, "primitives/3D/plane.3d", COLOR_3D_MAT),
    ActorTemplate(CUBEMAP_3D, PRIMITIVE_ACTOR, "primitives/3D/cubemap.3d", COLOR_3D_MAT),
    ActorTemplate(BACKPACK_MODEL, MODEL_ACTOR, "models/backpack/backpack.obj", MODEL_3D_MAT),
    ActorTemplate(CYBORG_MODEL, MODEL_ACTOR, "models/cyborg/cyborg.obj", MODEL_3D_MAT),
    ActorTemplate(NANOSUIT_MODEL, MODEL_ACTOR, "models/nanosuit/nanosuit.obj", MODEL_3D_MAT),
    ActorTemplate(PLANET_MODEL, MODEL_ACTOR, "models/planet/planet.obj", MODEL_3D_MAT),
    ActorTemplate(ROCK_MODEL, MODEL_ACTOR, "models/rock/rock.obj", MODEL_3D_MAT),
    ActorTemplate(SPHERE_MODEL, MODEL_ACTOR, "models/sphere/sphere.obj", MODEL_3D_MAT),
    ActorTemplate(TEAPOT_MODEL, MODEL_ACTOR, "models/teapot/teapot.obj", MODEL_3D_MAT),
    ActorTemplate(COLOR_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(BASE_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(NORMAL_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(POINT_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(DIR_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(SPT_LIGHT_ACTOR, LIGHT_ACTOR, " ", LIGHT_MAT),
    ActorTemplate(CAMERA_OBJECT_ACTOR, CAMERA_ACTOR, " ", COLOR_2D_MAT)};

Actor::Actor(std::string name_, Transform transform_)
    : name(name_), transform(transform_) {}

CameraActor::CameraActor(std::string name_, int id_, Transform transform_)
{
    name = name_;
    id = id_;
    cam = Camera(transform_.position);
    transform = Transform(cam.position, Vec3(0.0f), Vec3(1.0f));
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
        cam.position = transform.position;
        cam.yaw = transform.rotation.y + CAMERA_YAW;
        cam.pitch = clamp(transform.rotation.x, -CAMERA_MAX_PITCH, CAMERA_MAX_PITCH);
        cam.update_camera_vectors();
        cam.right = transform.get_right();
        cam.up = transform.get_up();
    }
    else
    {
        transform.position = cam.position;
        transform.rotation.y = overflow(cam.yaw - CAMERA_YAW, 360.0f);
        transform.rotation.x = cam.pitch;
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
    type = actorTemplates[template_].type;
    actor_template = template_;
    id = id_;
    path = resource_dir + actorTemplates[template_].path;
    isVisible = true;
    mat = material_template[actorTemplates[template_].matTemplate];
}
