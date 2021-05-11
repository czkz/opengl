#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include "Shader.h"

class Transform {
public:
    Vector3 position;
    Quaternion rotation;
    float scale = 1;

    void SetUniforms(ShaderProg& c) {
        c.SetVec3("u_object.position", position);
        c.SetQuaternion("u_object.rotation", rotation);
        c.SetFloat("u_object.scale", scale);
    }
};

