#include <object/Transform.h>

Transform::Transform()
{
    position = Vec3(0.0f);
    rotation = Vec3(0.0f);
    scale = Vec3(1.0f);
}

Transform::Transform(Vec3 pos, Vec3 rot, Vec3 scl)
{
    position = pos;
    rotation = rot;
    scale = scl;
}

void Transform::Reset()
{
    position = Vec3(0.0f);
    rotation = Vec3(0.0f);
    scale = Vec3(1.0f);
}