#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H

// glm libraries for math
#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>
#include <external/glm/glm/gtc/type_ptr.hpp>
#include <external/glm/glm/gtc/quaternion.hpp>
#include <external/glm/glm/gtx/quaternion.hpp>

class Transform
{
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Transform();
    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);
    void ResetToOrigin();

private:
};

#endif