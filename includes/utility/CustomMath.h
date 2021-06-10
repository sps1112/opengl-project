#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H

// glm libraries for math
#include <external/glm/glm/glm.hpp>
#include <external/glm/glm/gtc/matrix_transform.hpp>
#include <external/glm/glm/gtc/type_ptr.hpp>
#include <external/glm/glm/gtc/quaternion.hpp>
#include <external/glm/glm/gtx/quaternion.hpp>

#define PI_VAL 3.14159265

// Custom Math Definitions
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

// Custom Functions

// Clamps the value of a variable between some maximum and minimum value
float clamp(float a, float min, float max);
// Returns the max number b/w a and b
float max(float a, float b);
// Returns the min number b/w a and b
float min(float a, float b);
// Clamps the value of a b/w 0 and 1
float saturate(float a);
// Converts an Angle given in radians to degrees
float radian_to_degree(float rad);
// Converts an Angle given in degrees to radians
float degree_to_radian(float degree);

//Transform Class
class Transform
{
public:
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    // Standard Constructor for the Transform Class. Sets position at origin
    Transform();
    // Constructor for the Transform Class. Sets the position/rotation/scale at initialisation
    Transform(Vec3 pos, Vec3 rot, Vec3 scl);
    // Resets the transform the object and sets position at origin
    void Reset();

private:
};

#endif