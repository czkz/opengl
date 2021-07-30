#pragma once
#include <glad/glad.h>
#include <array>
#include <Vector.h>
#include <Quaternion.h>
#include "VertexResolver.h"
#include "registerAttributes.h"

namespace model_cube {
    struct vertex {
        Vector3 pos;
        Vector3 normal;
        Vector2 texCoord;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f, -0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 0.0f, 0.0f } },
        vertex { { +0.5f, +0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 1.0f, 1.0f } },
        vertex { { +0.5f, -0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, +0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 1.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, +0.5f, -0.5f }, { +0.0f, +0.0f, -1.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 0.0f, 0.0f } },
        vertex { { +0.5f, -0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 1.0f, 1.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 1.0f, 1.0f } },
        vertex { { -0.5f, +0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, +0.5f }, { +0.0f, +0.0f, +1.0f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, +0.5f, +0.5f }, { -1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { -0.5f, +0.5f, -0.5f }, { -1.0f, +0.0f, +0.0f }, { 1.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, +0.5f }, { -1.0f, +0.0f, +0.0f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, +0.5f, +0.5f }, { -1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, -0.5f, -0.5f }, { +1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { +0.5f, +0.5f, -0.5f }, { +1.0f, +0.0f, +0.0f }, { 1.0f, 1.0f } },
        vertex { { +0.5f, -0.5f, -0.5f }, { +1.0f, +0.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +1.0f, +0.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, -0.5f, +0.5f }, { +1.0f, +0.0f, +0.0f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { +0.0f, -1.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { +0.5f, -0.5f, -0.5f }, { +0.0f, -1.0f, +0.0f }, { 1.0f, 1.0f } },
        vertex { { +0.5f, -0.5f, +0.5f }, { +0.0f, -1.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, -0.5f, +0.5f }, { +0.0f, -1.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { -0.5f, -0.5f, +0.5f }, { +0.0f, -1.0f, +0.0f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { +0.0f, -1.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, +0.5f, -0.5f }, { +0.0f, +1.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +0.0f, +1.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { +0.5f, +0.5f, -0.5f }, { +0.0f, +1.0f, +0.0f }, { 1.0f, 1.0f } },
        vertex { { +0.5f, +0.5f, +0.5f }, { +0.0f, +1.0f, +0.0f }, { 1.0f, 0.0f } },
        vertex { { -0.5f, +0.5f, -0.5f }, { +0.0f, +1.0f, +0.0f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, +0.5f, +0.5f }, { +0.0f, +1.0f, +0.0f }, { 0.0f, 0.0f } },
    };
}
template <>
size_t registerAttributes<model_cube::vertex>(int i) {
    return VertexResolver::Register<Vector3, Vector3, Vector2>(i);
}

namespace model_plane_normals {
    struct vertex {
        Vector3 pos;
        Vector3 normal;
        Vector2 texCoord;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f,  0.0f, -0.5f },  { 0.0f,  1.0f,  0.0f },  { 0.0f, 1.0f } },
        vertex { {  0.5f,  0.0f, -0.5f },  { 0.0f,  1.0f,  0.0f },  { 1.0f, 1.0f } },
        vertex { {  0.5f,  0.0f,  0.5f },  { 0.0f,  1.0f,  0.0f },  { 1.0f, 0.0f } },
        vertex { {  0.5f,  0.0f,  0.5f },  { 0.0f,  1.0f,  0.0f },  { 1.0f, 0.0f } },
        vertex { { -0.5f,  0.0f,  0.5f },  { 0.0f,  1.0f,  0.0f },  { 0.0f, 0.0f } },
        vertex { { -0.5f,  0.0f, -0.5f },  { 0.0f,  1.0f,  0.0f },  { 0.0f, 1.0f } }
    };
}
template <>
size_t registerAttributes<model_plane_normals::vertex>(int i) {
    return VertexResolver::Register<Vector3, Vector3, Vector2>(i);
}

namespace model_ndc_quad {
    struct vertex {
        Vector2 pos;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -1.0f, -1.0f } },
        vertex { {  1.0f, -1.0f } },
        vertex { { -1.0f,  1.0f } },
        vertex { {  1.0f, -1.0f } },
        vertex { {  1.0f,  1.0f } },
        vertex { { -1.0f,  1.0f } },
    };
}
template <>
size_t registerAttributes<model_ndc_quad::vertex>(int i) {
    return VertexResolver::Register<Vector3, Vector3, Vector2>(i);
}

namespace model_ndc_points {
    struct vertex {
        Vector2 pos;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f, -0.5f } },
        vertex { {  0.5f, -0.5f } },
        vertex { {  0.5f,  0.5f } },
        vertex { { -0.5f,  0.5f } },
    };
}
template <>
size_t registerAttributes<model_ndc_points::vertex>(int i) {
    return VertexResolver::Register<Vector3, Vector3, Vector2>(i);
}

namespace model_skybox_cube {
    struct vertex {
        Vector3 pos;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { {-1.0f,  1.0f, -1.0f} },
        vertex { {-1.0f, -1.0f, -1.0f} },
        vertex { { 1.0f, -1.0f, -1.0f} },
        vertex { { 1.0f, -1.0f, -1.0f} },
        vertex { { 1.0f,  1.0f, -1.0f} },
        vertex { {-1.0f,  1.0f, -1.0f} },

        vertex { {-1.0f, -1.0f,  1.0f} },
        vertex { {-1.0f, -1.0f, -1.0f} },
        vertex { {-1.0f,  1.0f, -1.0f} },
        vertex { {-1.0f,  1.0f, -1.0f} },
        vertex { {-1.0f,  1.0f,  1.0f} },
        vertex { {-1.0f, -1.0f,  1.0f} },

        vertex { { 1.0f, -1.0f, -1.0f} },
        vertex { { 1.0f, -1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f, -1.0f} },
        vertex { { 1.0f, -1.0f, -1.0f} },

        vertex { {-1.0f, -1.0f,  1.0f} },
        vertex { {-1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f, -1.0f,  1.0f} },
        vertex { {-1.0f, -1.0f,  1.0f} },

        vertex { {-1.0f,  1.0f, -1.0f} },
        vertex { { 1.0f,  1.0f, -1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { { 1.0f,  1.0f,  1.0f} },
        vertex { {-1.0f,  1.0f,  1.0f} },
        vertex { {-1.0f,  1.0f, -1.0f} },

        vertex { {-1.0f, -1.0f, -1.0f} },
        vertex { {-1.0f, -1.0f,  1.0f} },
        vertex { { 1.0f, -1.0f, -1.0f} },
        vertex { { 1.0f, -1.0f, -1.0f} },
        vertex { {-1.0f, -1.0f,  1.0f} },
        vertex { { 1.0f, -1.0f,  1.0f} }
    };
}
template <>
size_t registerAttributes<model_skybox_cube::vertex>(int i) {
    return VertexResolver::Register<Vector3, Vector3, Vector2>(i);
}

namespace model_point {
    struct vertex {
        Vector3 pos;
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { 0.0f, 0.0f, 0.0f } },
    };
}
template <>
size_t registerAttributes<model_point::vertex>(int i) {
    return VertexResolver::Register<Vector3>(i);
}

