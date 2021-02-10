#pragma once
#include <functional>
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

struct Object {
    Model& model;
    Transform transform;
};
