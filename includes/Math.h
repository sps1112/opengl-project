#ifndef MATH_H
#define MATH_H

// glm libraries for math
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>

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