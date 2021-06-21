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

Vec3 Transform::GetFront()
{
    Vec3 front = -WORLD_FORWARD;
    Mat4 rotMat(1.0f);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.y), WORLD_UP);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.x), WORLD_RIGHT);
    front = Vec3(rotMat * Vec4(front, 1.0f));
    return glm::normalize(front);
}

Vec3 Transform::GetRight()
{
    Vec3 right = WORLD_RIGHT;
    Mat4 rotMat(1.0f);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.y), WORLD_UP);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.z), WORLD_FORWARD);
    right = Vec3(rotMat * Vec4(right, 1.0f));
    return glm::normalize(right);
}

Vec3 Transform::GetUp()
{
    Vec3 up = WORLD_UP;
    Mat4 rotMat(1.0f);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.z), WORLD_FORWARD);
    rotMat = glm::rotate(rotMat, glm::radians(rotation.x), WORLD_RIGHT);
    up = Vec3(rotMat * Vec4(up, 1.0f));
    return glm::normalize(up);
}