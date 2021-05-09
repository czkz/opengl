#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include "GLObjectHandles.h"

class BufferObject {
protected:
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
    void LoadStruct(
        const auto& data,
        GLenum usage
    ) {
        std::span _data (&data, 1);
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
class UBO : public BufferObject {
public:
    UBO() : BufferObject(GL_UNIFORM_BUFFER) { }
    UBO(auto&&... args) : BufferObject(GL_UNIFORM_BUFFER, args...) { }

    void BindingPoint(GLuint point) {
        glBindBufferBase(GL_UNIFORM_BUFFER, point, handle.value);
    }
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

