#pragma once
#include <Vector.h>
#include <Quaternion.h>
#include <numbers>


namespace math::_ {

    void generate_quad(Vector2 a, Vector2 b, std::vector<Vector3>& out) {
        out.emplace_back( a.x, a.y, 0.5 );
        out.emplace_back( b.x, a.y, 0.5 );
        out.emplace_back( a.x, b.y, 0.5 );
        out.emplace_back( a.x, b.y, 0.5 );
        out.emplace_back( b.x, a.y, 0.5 );
        out.emplace_back( b.x, b.y, 0.5 );
    }

    void generate_side(int sub, std::vector<Vector3>& out) {
        for (int i = 0; i < sub; i++) {
            for (int j = 0; j < sub; j++) {
                generate_quad(
                    {float(i+0)/sub-0.5f, float(j+0)/sub-0.5f},
                    {float(i+1)/sub-0.5f, float(j+1)/sub-0.5f},
                    out
                );
            }
        }
    }

    void generate_cube_pos(std::vector<Vector3>& verts, int subdivisions) {
        size_t side_verts = 6 * subdivisions * subdivisions;
        verts.reserve(side_verts * 6);
        auto rotations = std::to_array({
            Quaternion::Identity(),
            Quaternion::RotationN(std::numbers::pi, {0, +1, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {+1, 0, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {-1, 0, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {0, +1, 0}),
            Quaternion::RotationN(std::numbers::pi / 2, {0, -1, 0}),
        });
        // Generate side 0
        generate_side(subdivisions, verts);
        // Generate others
        for (int side = 1; side < 6; side++) {
            for (size_t i = 0; i < side_verts; i++) {
                verts.push_back(rotations[side].Rotate(verts[i]));
            }
        }
    }

    void generate_cube_normals(std::vector<Vector3>& norms, int subdivisions) {
        size_t side_verts = 6 * subdivisions * subdivisions;
        norms.reserve(side_verts * 6);
        auto normals = std::to_array({
            Vector3 { 0, 0, +1 },
            Vector3 { 0, 0, -1 },
            Vector3 { 0, -1, 0 },
            Vector3 { 0, +1, 0 },
            Vector3 { +1, 0, 0 },
            Vector3 { -1, 0, 0 },
        });
        for (int side = 0; side < 6; side++) {
            for (size_t i = 0; i < side_verts; i++) {
                norms.push_back(normals[side]);
            }
        }
    }

    void generate_sphere_normals(std::vector<Vector3>& verts, std::vector<Vector3>& norms, int subdivisions) {
        size_t side_verts = 6 * subdivisions * subdivisions;
        norms.reserve(side_verts * 6);
        for (const auto& pos : verts) {
            norms.push_back(pos);
        }
    }

    void generate_cube_uvs(std::vector<Vector2>& uvs, int subdivisions) {
        size_t side_verts = 6 * subdivisions * subdivisions;
        uvs.reserve(side_verts * 6);
        // Generate side 0
        std::vector<Vector3> side_pos;
        side_pos.reserve(side_verts);
        generate_side(subdivisions, side_pos);
        for (const auto& v : side_pos) {
            uvs.emplace_back(v.x, v.y);
        }
        // Generate others
        for (int side = 1; side < 6; side++) {
            for (size_t i = 0; i < side_verts; i++) {
                uvs.push_back(uvs[i]);
            }
        }
    }

    void generate_sphere_uvs(
        std::ranges::input_range auto&& in_vertex_pos,
        std::vector<Vector2>& out_uvs
    ) {
        using std::numbers::pi;
        out_uvs.reserve(std::size(in_vertex_pos));
        for (const auto& v : in_vertex_pos) {
            out_uvs.emplace_back(
                std::atan2(v.y, v.x) / pi * 0.5 + 0.5,
                std::asin(v.z) / pi + 0.5
            );
        }
    }

}

namespace math {

    void generate_cube(std::vector<Vector3>& verts, std::vector<Vector3>& norms, int subdivisions) {
        _::generate_cube_pos(verts, subdivisions);
        _::generate_cube_normals(norms, subdivisions);
    }

    void generate_sphere(std::vector<Vector3>& verts, std::vector<Vector3>& norms, int subdivisions) {
        _::generate_cube_pos(verts, subdivisions);
        for (auto& pos : verts) {
            pos.Normalize();
        }
        _::generate_sphere_normals(verts, norms, subdivisions);
    }

}
