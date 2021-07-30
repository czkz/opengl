#pragma once
#include "registerAttributes.h"
#include "VertexResolver.h"
#include <cstddef>
#include "Transform.h"

template <>
size_t registerAttributes<Transform>(int i) {
    static_assert(offsetof(Transform, rotation) == 3*4, "Expecting position before rotation");
    return VertexResolver::Register<Vector3, Quaternion, float>(i);
}
