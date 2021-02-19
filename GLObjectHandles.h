#pragma once
#include <glad/glad.h>
#include <utility>


class GLObjectHandle {
protected:
    GLObjectHandle() = default;
public:
    GLuint value = 0;
    GLObjectHandle(const GLObjectHandle&) = delete;
    GLObjectHandle(GLObjectHandle&&) = default;
    GLObjectHandle& operator=(GLObjectHandle&& other) {
        std::swap(*this, other);
        return *this;
    }
};

class BufferObjectHandle : public GLObjectHandle {
public:
    BufferObjectHandle(BufferObjectHandle&&) = default;
    BufferObjectHandle() {
        glGenBuffers(1, &value);
    }
    ~BufferObjectHandle() {
        glDeleteBuffers(1, &value);
    }
};

class VAOHandle : public GLObjectHandle {
public:
    VAOHandle(VAOHandle&&) = default;
    VAOHandle() {
        glGenVertexArrays(1, &value);
    }
    ~VAOHandle() {
        glDeleteVertexArrays(1, &value);
    }
};

