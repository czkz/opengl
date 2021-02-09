#pragma once
#include "Transform.h"
#include "Model.h"

struct Object {
    Object(Model& m, Transform t = {{0, 0, 0}, Quaternion::Identity()}) : model(m), transform(t) { }
    Model& model;
    Transform transform;
};
