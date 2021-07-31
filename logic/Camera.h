#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <algorithm>

class SpaceCamera {
public:
    Vector3 targetPosition;
    Vector3 currentPosition;
    Quaternion rotation;

    SpaceCamera(Vector3 position, Quaternion rotation)
        : targetPosition(position), currentPosition(position), rotation(rotation)
    { }

    const Quaternion& getRotation() const {
        return rotation;
    }

    void RotateX(float ang) {
        rotation = rotation * Quaternion::Rotation(ang, {1, 0, 0});
    }

    void RotateY(float ang) {
        rotation = rotation * Quaternion::Rotation(ang, {0, 1, 0});
    }

    void RotateZ(float ang) {
        rotation = rotation * Quaternion::Rotation(ang, {0, 0, 1});
    }

    void Move(const Vector3& localSpaceTranslation) {
        targetPosition += rotation.Rotate(localSpaceTranslation);
    }

    void onTick(float deltaTime) {
        static constexpr float t = 0.1;
        currentPosition = Vector3::Lerp(currentPosition, targetPosition, 1 - pow(1 - t, deltaTime));
    }
};

