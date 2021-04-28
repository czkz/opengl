#pragma once
#include <glad/glad.h>
#include <array>
#include <Vector.h>
#include <Quaternion.h>

class VertexResolver {
    static consteval std::pair<GLint, GLenum> OpenGL_attributeType(int*)        { return std::make_pair(1, GL_INT); }
    static consteval std::pair<GLint, GLenum> OpenGL_attributeType(float*)      { return std::make_pair(1, GL_FLOAT); }
    static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Vector3*)    { return std::make_pair(3, GL_FLOAT); }
    static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Vector2*)    { return std::make_pair(2, GL_FLOAT); }
    static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Quaternion*) { return std::make_pair(3, GL_FLOAT); }

    template <typename Curr, typename... Ts>
    static size_t _registerAttributes(int i = 0, size_t bytes = 0) {
        std::pair<GLint, GLenum> c = OpenGL_attributeType((Curr*) nullptr);
        size_t knownStride = bytes + sizeof(Curr);
        if constexpr (sizeof...(Ts) != 0) {
            knownStride = _registerAttributes<Ts...>(i + 1, knownStride);
        }
        glVertexAttribPointer(i, c.first, c.second, GL_FALSE, knownStride, (void*) bytes);
        return knownStride;
    }

public:
    template <typename... Ts>
    static size_t Register() {
        _registerAttributes<Ts...>();
        return sizeof...(Ts);
    }
};

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

namespace model_screen_quad {
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

