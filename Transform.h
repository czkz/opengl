#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include "Shader.h"

class Transform {
public:
    Vector3 position;
    Quaternion rotation;
    float scale = 1;

    void SetUniforms(ShaderProg::Config c) {
        c.SetVec3("objectPosition", position);
        c.SetQuaternion("objectRotation", rotation);
        c.SetFloat("objectScale", scale);
    }
};

