#ifndef MATERIAL_H
#define MATERIAL_H

#include <utility/Utils.h>
#include <utility/CustomMath.h>

using Colorf = Vec3;

enum MATERIAL_TYPE
{
    DIFFUSE_MAT,
    SPECULAR_MAT,
};

struct Material
{
    Colorf albedo;
    Colorf emmision;
    MATERIAL_TYPE type;

    Material(Colorf _albedo, Colorf _emmision, MATERIAL_TYPE _type)
        : albedo(_albedo), emmision(_emmision), type(_type) {}
};

#endif