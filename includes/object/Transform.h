#ifndef TRANSFORM_H
#define TRANSFORM_H

// Header declarations
#include <utility/CustomMath.h>
#include <config.h>

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
    // Gets Local Front Vector
    Vec3 GetFront();
    // Gets Local Right Vector
    Vec3 GetRight();
    // Gets Local Up Vector
    Vec3 GetUp();

private:
};

#endif // !TRANSFORM_H
