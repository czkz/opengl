#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <numbers>


namespace {

    void _generate_quad(Vector2 a, Vector2 b, std::vector<Vector3>& out) {
        out.emplace_back( a.x, a.y, 0.5 );
        out.emplace_back( a.x, b.y, 0.5 );
        out.emplace_back( b.x, a.y, 0.5 );
        out.emplace_back( b.x, a.y, 0.5 );
        out.emplace_back( a.x, b.y, 0.5 );
        out.emplace_back( b.x, b.y, 0.5 );
    }

    void _generate_side(int sub, std::vector<Vector3>& out) {
        for (int i = 0; i < sub; i++) {
            for (int j = 0; j < sub; j++) {
                _generate_quad(
                    {float(i+0)/sub-0.5f, float(j+0)/sub-0.5f},
                    {float(i+1)/sub-0.5f, float(j+1)/sub-0.5f},
                    out
                );
            }
        }
    }

}

namespace math {

    void generate_cube(std::vector<Vector3>& verts, std::vector<Vector3>& norms, int subdivisions) {
        verts.reserve(4 * subdivisions * subdivisions * 6);
        norms.reserve(4 * subdivisions * subdivisions * 6);
        _generate_side(subdivisions, verts);
        size_t side_verts = verts.size();
        auto rotations = std::to_array({
            Quaternion::Identity(),
            Quaternion::RotationN(std::numbers::pi, {0, +1, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {+1, 0, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {-1, 0, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {0, +1, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {0, -1, 0}),
        });
        auto normals = std::to_array({
            Vector3 { 0, 0, +1 },
            Vector3 { 0, 0, -1 },
            Vector3 { 0, -1, 0 },
            Vector3 { 0, +1, 0 },
            Vector3 { +1, 0, 0 },
            Vector3 { -1, 0, 0 },
        });
        for (int side = 1; side < 6; side++) {
            for (size_t i = 0; i < side_verts; i++) {
                verts.push_back(rotations[side].Rotate(verts[i]));
            }
        }
        for (int side = 0; side < 6; side++) {
            for (size_t i = 0; i < side_verts; i++) {
                norms.push_back(normals[side]);
            }
        }
    }

}
