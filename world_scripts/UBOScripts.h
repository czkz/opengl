#pragma once
#include "Camera.h"

namespace UBOStruct {

    struct Camera {
        alignas(16) Quaternion rotation;
        alignas(16) Vector3 position;
    };
    Camera make_camera(const auto& camera) {
        return {
            camera.getRotation(),
            camera.position
        };
    }

}
