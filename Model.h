#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include <functional>
#include "GLObjectHandles.h"

class BufferObject {
    BufferObjectHandle handle;
    size_t last_n_elems = 0;
public:
    const GLenum type;
    /// See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
    BufferObject(GLenum type) : type(type) { }
    BufferObject(BufferObject&&) = default;

    BufferObject(
        GLenum type,
        const std::ranges::range auto& data,
        GLenum usage = GL_STATIC_DRAW
    ) : BufferObject(type) { LoadData(data, usage); }

    size_t size() const { return last_n_elems; }

    void LoadData(
        const std::ranges::range auto& data,
        GLenum usage = GL_STATIC_DRAW
    ) {
        std::span _data = data;
        Bind();
        glBufferData(type, _data.size_bytes(), _data.data(), usage);
        last_n_elems = _data.size();
    }

public:
    // friend class VAO;
    void Bind() {
        glBindBuffer(type, handle.value);
    }
};

class VBO : public BufferObject {
public:
    VBO() : BufferObject(GL_ARRAY_BUFFER) { }
    VBO(auto&&... args) : BufferObject(GL_ARRAY_BUFFER, args...) { }
};
class EBO : public BufferObject {
public:
    EBO() : BufferObject(GL_ELEMENT_ARRAY_BUFFER) { }
    EBO(auto&&... args) : BufferObject(GL_ELEMENT_ARRAY_BUFFER, args...) { }
};


class VAO {
    VAOHandle handle;
public:
    VAO() = default;

    /// VAO_lock is just to make it harder to forget
    void Bind() {
        glBindVertexArray(handle.value);
    }

    static void Unbind() {
        glBindVertexArray(0);
    }
};


// class SimpleModel {
// public:
//     VAO vao;
//     VBO vbo;
//
//     SimpleModel(const std::ranges::range auto& vertices) {
//         vbo.LoadData(vertices);
//         vao.Configure([this](VAO::Config c) {
//             c.Bind(vbo);
//             ///TODO automate, make static
//             size_t nAttrs = std::remove_cvref<decltype(vertices[0])>::type::registerAttributes();
//             for (size_t i = 0; i < nAttrs; i++) {
//                 glEnableVertexAttribArray(i);
//             }
//         });
//     }
//
//     void Draw(VAO_lock& lock) {
//         vao.Bind(lock);
//         glDrawArrays(GL_TRIANGLES, 0, vbo.size());
//     }
// };

