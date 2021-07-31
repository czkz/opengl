#pragma once
#include "Camera.h"
#include "Transform.h"

namespace UBOStruct {

    struct Camera {
        alignas(16) Quaternion rotation;
        alignas(16) Vector3 position;
    };
    inline Camera make_camera(const auto& v) {
        return {
            v.getRotation(),
            v.currentPosition
        };
    }

    struct Transform {
        alignas(16) Quaternion rotation;
        alignas(16) Vector3 position;
        alignas(4) float scale;
    };
    inline Transform make_transform(const ::Transform& v) {
        return {
            v.rotation,
            v.position,
            v.scale
        };
    }

}
