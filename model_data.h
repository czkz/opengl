#pragma once
#include <glad/glad.h>
#include <array>
#include <Vector.h>
#include <Quaternion.h>
#include "VertexResolver.h"

namespace model_cube_textured {
    struct vertex {
        Vector3 pos;
        Vector2 texCoord;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3, Vector2>();
        }
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } } 
    };
}

namespace model_cube_normals {
    struct vertex {
        Vector3 pos;
        Vector3 normal;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3, Vector3>();
        }
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { {  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { { -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
        vertex { {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        vertex { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        vertex { { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
        vertex { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
    };
}

namespace model_plane_normals {
    struct vertex {
        Vector3 pos;
        Vector3 normal;
        Vector2 texCoord;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3, Vector3, Vector2>();
        }
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

namespace model_ndc_quad {
    struct vertex {
        Vector2 pos;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector2>();
        }
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

namespace model_ndc_points {
    struct vertex {
        Vector2 pos;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector2>();
        }
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -0.5f, -0.5f } },
        vertex { {  0.5f, -0.5f } },
        vertex { {  0.5f,  0.5f } },
        vertex { { -0.5f,  0.5f } },
    };
}

namespace model_skybox_cube {
    struct vertex {
        Vector3 pos;

        static size_t registerAttributes() {
            return VertexResolver::Register<Vector3>();
        }
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

