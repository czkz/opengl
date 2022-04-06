#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <Matrix.h>

struct Transform {
    Vector3 position;
    Quaternion rotation = Quaternion::Identity();
    Vector3 scale = Vector3(1);

    MatrixS<4, 4> Matrix() const {
        return
            Vector3(position).TranslationMatrix() *
            rotation.RotationMatrix() *
            scale.ScaleMatrix();
    }
};
