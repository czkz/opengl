#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <array>

namespace model_cube_normals {
    struct vertex {
        Vector3 pos;
        Vector2 texCoord;

        static size_t registerAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0                   );
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)) );
            return 2;
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

namespace model_plane_normals {
    struct vertex {
        Vector3 pos;
        Vector3 normal;
        Vector2 texCoord;

        static size_t registerAttributes() {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0                   );
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)) );
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)) );
            return 3;
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

namespace model_screen_quad {
    struct vertex {
        Vector2 pos;

        static size_t registerAttributes() {
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
            return 1;
        }
    };

    [[maybe_unused]]
    constexpr std::array vertices = {
        vertex { { -1.0f, -1.0f } },
        vertex { { -1.0f,  1.0f } },
        vertex { {  1.0f, -1.0f } },
        vertex { {  1.0f, -1.0f } },
        vertex { { -1.0f,  1.0f } },
        vertex { {  1.0f,  1.0f } }
    };
}

