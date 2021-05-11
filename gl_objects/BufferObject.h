#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include "GLObjectHandles.h"

class BufferObject {
protected:
    BufferObjectHandle handle;
    size_t last_n_elems = 0;
    size_t bytes_allocated = 0;

    template <typename T, size_t N>
    void loadData(std::span<T, N> data,
                  GLenum usage,
                  bool force_realloc)
    {
        Bind();
        if (force_realloc || bytes_allocated < data.size_bytes()) {
            glBufferData(type, data.size_bytes(), data.data(), usage);
            bytes_allocated = data.size_bytes();
        } else {
            glBufferSubData(type, 0, data.size_bytes(), data.data());
        }
        last_n_elems = data.size();
    }
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
    size_t size_bytes() const { return bytes_allocated; }

    /// @arg usage unused if storage is not reallocated
    void LoadData(const std::ranges::range auto& data, GLenum usage = GL_STATIC_DRAW) {
        return loadData(std::span(data), usage, false);
    }
    void LoadData_ForceRealloc(const std::ranges::range auto& data, GLenum usage) {
        return loadData(std::span(data), usage, true);
    }

    /// @arg usage unused if storage is not reallocated
    void LoadStruct(const auto& value, GLenum usage) {
        return loadData(std::span(&value, 1), usage, false);
    }
    void LoadStruct_ForceRealloc(const auto& value, GLenum usage) {
        return loadData(std::span(&value, 1), usage, true);
    }

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

    void Bind() {
        glBindVertexArray(handle.value);
    }

    static void Unbind() {
        glBindVertexArray(0);
    }
};

