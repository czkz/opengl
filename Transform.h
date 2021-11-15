#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <Matrix.h>

struct Transform {
    Vector3 position;
    Quaternion rotation;
    float scale = 1;

    MatrixS<4, 4> Matrix() const {
        return
            Vector3(position).TranslationMatrix() *
            rotation.RotationMatrix() *
            Vector3(scale).ScaleMatrix();
    }
};
