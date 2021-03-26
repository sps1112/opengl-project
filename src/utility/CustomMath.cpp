#include <utility/CustomMath.h>

Transform::Transform()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
}

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
    position = pos;
    rotation = rot;
    scale = scl;
}

void Transform::ResetToOrigin()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
}