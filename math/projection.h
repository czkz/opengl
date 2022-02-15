#pragma once
#include <Matrix.h>
#include <numbers>
#include <cmath>

namespace math {

    MatrixS<4, 4> projection(float fov_deg, float aspect) {
        const float fov = fov_deg * std::numbers::pi / 180.0;
        const float n = 0.01;
        const float f = 100.;
        const float ff = 1.0/tan(fov/2.0);
        // Expects z to point up
        return MatrixS<4, 4>({
            ff / aspect, 0,           0,  0,
            0,           0,           ff, 0,
            0,           (f+n)/(f-n), 0,  -2.f*f*n/(f-n),
            0,           1,          0,  0
        });
        // Normal (z points back)
        // return MatrixS<4, 4>({
        //     ff / aspect, 0,  0,            0,
        //     0,           ff, 0,            0,
        //     0,           0,  -(f+n)/(f-n), -2.f*f*n/(f-n),
        //     0,           0,  -1,           0
        // });
    }

}
