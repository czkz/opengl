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

protected:
    friend class VAO;
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



/// Unbinds any VAOs in destructor
class VAO_lock {
public:
    ~VAO_lock() {
        glBindVertexArray(0);
    }
};

class VAO {
    VAOHandle handle;
public:
    class Config {
    public:
        void Bind(BufferObject& bo) {
            bo.Bind();
        }
    };

    VAO() = default;
    VAO(const std::function<void(Config)>& config_script) {
        Configure(config_script);
    }

    void Configure(const std::function<void(Config)>& config_script) {
        VAO_lock lock;
        Bind(lock);
        config_script(Config());
    }

    /// VAO_lock is just to make it harder to forget
    void Bind(VAO_lock&) {
        glBindVertexArray(handle.value);
    }
};


class SimpleModel {
public:
    VAO vao;
    VBO vbo;

    SimpleModel(const std::ranges::range auto& vertices) {
        vbo.LoadData(vertices);
        vao.Configure([this](VAO::Config c) {
            c.Bind(vbo);
            ///TODO automate, make static
            size_t nAttrs = std::remove_cvref<decltype(vertices[0])>::type::registerAttributes();
            for (size_t i = 0; i < nAttrs; i++) {
                glEnableVertexAttribArray(i);
            }
        });
    }

    void Draw(VAO_lock& lock) {
        vao.Bind(lock);
        glDrawArrays(GL_TRIANGLES, 0, vbo.size());
    }
};


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

