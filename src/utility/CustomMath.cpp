#include <utility/CustomMath.h>

float clamp(float a, float min, float max)
{
    return glm::clamp(a, min, max);
}

float max(float a, float b)
{
    return ((a > b) ? a : b);
}

float min(float a, float b)
{
    return ((a > b) ? a : b);
}

float saturate(float a)
{
    return clamp(a, 0.0f, 1.0f);
}

float overflow(float a, float period)
{
    float min = -period / 2.0f;
    float max = period / 2.0f;
    if (a > max)
    {
        float diff = a - max;
        int multi = (int)(diff / period);
        float extra = diff - (multi * period);
        return (min + extra);
    }
    else if (a < min)
    {
        float diff = min - a;
        int multi = (int)(diff / period);
        float extra = diff - (multi * period);
        return (max - extra);
    }
    return a;
}

float floor(float a, float val, float diff)
{
    return ((a <= val) ? (val + diff) : a);
}

float ciel(float a, float val, float diff)
{
    return ((a >= val) ? (val - diff) : a);
}

float radian_to_degree(float rad)
{
    return ((rad / PI_VAL) * 180.0f);
}

float degree_to_radian(float degree)
{
    return ((degree / 180.0f) * PI_VAL);
}