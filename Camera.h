#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <algorithm>

class SpaceCamera {
public:
    Vector3 position;
    Quaternion rotation;

    const Quaternion& getRotation() const {
        return rotation;
    }

    void RotateX(float ang) {
        rotation = Quaternion::Rotation(ang, {1, 0, 0}) * rotation;
    }

    void RotateY(float ang) {
        rotation = Quaternion::Rotation(ang, {0, 1, 0}) * rotation;
    }

    void RotateZ(float ang) {
        rotation = Quaternion::Rotation(ang, {0, 0, 1}) * rotation;
    }

    void Move(const Vector3& localSpaceTranslation) {
        position += rotation.Inverse().Rotate(localSpaceTranslation);
    }
};

class FPSCamera {
public:
    Vector3 position;
    Vector3 euler;
    /// Negative = no limit
    float maxPitch = 90 / 180.f * M_PI;

    const Quaternion getRotation() const {
        return Quaternion::Euler(euler);
    }

    void ClampPitch() {
        if (maxPitch > 0) {
            euler.x = std::clamp<VEC_REAL_T>(euler.x, -maxPitch, +maxPitch);
        }
    }

    void Move(Vector3 localMovement) {
        Vector3& v = localMovement;
        const Vector2 i (cos(euler.y), sin(euler.y));
        position += Vector3(v.x*i.x - v.z*i.y, v.y, v.z*i.x + v.x*i.y);
    }
};

