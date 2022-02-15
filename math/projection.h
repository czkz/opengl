#pragma once
#include <Matrix.h>
#include <numbers>
#include <cmath>

namespace math::_ {

    // Converts from right-handed z-up coordinate system
    // to right-handed z-back coordinate system expected in clip space
    constexpr MatrixS<4, 4> z_convert = {{
        1,  0, 0, 0,
        0,  0, 1, 0,
        0, -1, 0, 0,
        0,  0, 0, 1,
    }};

}

namespace math {

    MatrixS<4, 4> projection_perspective(float fov_deg, float aspect) {
        const float fov = fov_deg * std::numbers::pi / 180.0;
        const float n = 0.01;
        const float f = 100;
        const float ff = 1.0/tan(fov/2.0);
        return MatrixS<4, 4>({
            ff / aspect, 0,  0,            0,
            0,           ff, 0,            0,
            0,           0,  -(f+n)/(f-n), -2.f*f*n/(f-n),
            0,           0,  -1,           0
        }) * _::z_convert;
    }

    MatrixS<4, 4> projection_orthgraphic(float height, float aspect) {
        const float h2 = height / 2;
        const float w2 = h2 * aspect;
        const float n = 0;
        const float f = 100;
        return MatrixS<4, 4>({
            1/w2, 0,    0,        0,
            0,    1/h2, 0,        0,
            0,    0,    -2/(f-n), -(f+n)/(f-n),
            0,    0,    0,        1,
        }) * _::z_convert;
    }

}
