#pragma once
#include <memory>
#include "Transform.h"
#include "Model.h"

struct Object {
    std::shared_ptr<VAO> vao;
    Transform transform;
};
