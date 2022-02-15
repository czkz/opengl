#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <Quaternion.h>

namespace gl::_ {

    class VertexResolver {
        static consteval std::pair<GLint, GLenum> OpenGL_attributeType(int*)        { return std::make_pair(1, GL_INT); }
        static consteval std::pair<GLint, GLenum> OpenGL_attributeType(float*)      { return std::make_pair(1, GL_FLOAT); }
        static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Vector3*)    { return std::make_pair(3, GL_FLOAT); }
        static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Vector2*)    { return std::make_pair(2, GL_FLOAT); }
        static consteval std::pair<GLint, GLenum> OpenGL_attributeType(Quaternion*) { return std::make_pair(4, GL_FLOAT); }

        template <typename Curr, typename... Ts>
        static size_t _registerAttributes(int i, size_t bytes, bool enable) {
            std::pair<GLint, GLenum> c = OpenGL_attributeType((Curr*) nullptr);
            size_t knownStride = bytes + sizeof(Curr);
            if constexpr (sizeof...(Ts) != 0) {
                knownStride = _registerAttributes<Ts...>(i + 1, knownStride, enable);
            }
            glVertexAttribPointer(i, c.first, c.second, GL_FALSE, knownStride, (void*) bytes);
            if (enable) { glEnableVertexAttribArray(i); }
            return knownStride;
        }

    public:
        template <typename... Ts>
        static size_t Register(int i, bool enable) {
            _registerAttributes<Ts...>(i, 0, enable);
            return sizeof...(Ts);
        }
    };

}

namespace gl::_ {

    template <typename... Ts>
    static size_t register_attributes(int i, bool enable) {
        return _::VertexResolver::Register<Ts...>(i, enable);
    }

}
