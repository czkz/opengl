#pragma once
#include <Vector.h>
#include <Quaternion.h>

class SpaceCamera {
public:
    Vector3 position;
    Quaternion rotation;

    const Quaternion& getRotation() const {
        return rotation;
    }

    void RotateX(float pitch) {
        rotation = Quaternion::Rotation(pitch, {1, 0, 0}) * rotation;
    }

    void RotateY(float yaw) {
        rotation = Quaternion::Rotation(yaw, {0, 1, 0}) * rotation;
    }

    void RotateZ(float roll) {
        rotation = Quaternion::Rotation(roll, {0, 0, 1}) * rotation;
    }

    void Move(const Vector3& localSpaceTranslation) {
        position += rotation.Inverse().Rotate(localSpaceTranslation);
    }
};

class FPSCamera {
public:
    Vector3 position;
    Vector3 euler;

    const Quaternion getRotation() const {
        return Quaternion::Euler(euler);
    }

    void Move(Vector3 localMovement) {
        Vector3& v = localMovement;
        const Vector2 i (cos(euler.y), sin(euler.y));
        position += Vector3(v.x*i.x - v.z*i.y, v.y, v.z*i.x + v.x*i.y);
    }
};

