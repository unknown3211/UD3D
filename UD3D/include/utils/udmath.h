#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "globals/global_types.h"
#include "globals/global_structs.h"
#include <iostream>

inline float degToRad(float a)
{
	return a * M_PI / 180.0f;
}

inline float Distance2D(Vector2 a, Vector2 b)
{
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

inline float Distance3D(Vector3 a, Vector3 b)
{
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}

static float NormalizeAngle(float angle)
{
    while (angle > 180.0f) angle -= 360.0f;
    while (angle <= -180.0f) angle += 360.0f;
    return angle;
}

static float Rotate(float _angle)
{
    if (_angle > 360.0f)
        _angle -= 360.0f;

    return _angle;
}