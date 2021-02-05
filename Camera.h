#pragma once
#include <Vector.h>
#include <Quaternion.h>

class Camera {
public:
    Vector3 position;
    Quaternion rotation;

    void RotateX(float pitch) {
        rotation = Quaternion::Rotation(pitch, {1, 0, 0}) * rotation;
    }

    void RotateY(float yaw) {
        rotation = Quaternion::Rotation(yaw, {0, 1, 0}) * rotation;
    }

    void RotateZ(float roll) {
        rotation = Quaternion::Rotation(roll, {0, 0, 1}) * rotation;
    }

    void Translate(const Vector3& localSpaceTranslation) {
        position += rotation.Inverse().Rotate(localSpaceTranslation);
    }

    void ChangeYaw(float yawOffset) {
        rotation = rotation * Quaternion::Rotation(yawOffset, {0, 1, 0});
    }
};
