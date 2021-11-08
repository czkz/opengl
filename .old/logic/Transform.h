#pragma once
#include <Vector.h>
#include <Quaternion.h>

struct Transform {
    Vector3 position;
    Quaternion rotation;
    float scale = 1;
};

