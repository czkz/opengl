#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include <functional>

struct vertex {
    Vector3 pos;
    Vector3 color;
    Vector2 texCoord;

    static size_t registerAttributes() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0                   );
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)) );
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)) );
        return 3;
    }
};

class BufferObject {
    size_t last_n_elems = 0;
public:
    GLuint handle;
    const GLenum type;
    BufferObject(GLenum type) : type(type) {
        glGenBuffers(1, &handle);
    }
    ~BufferObject() {
        glDeleteBuffers(1, &handle);
    }
    BufferObject(const BufferObject&) = delete;
    BufferObject(BufferObject&&) = default;

    size_t size() const { return last_n_elems; }

    BufferObject& LoadData(const std::ranges::range auto& data, GLenum usage = GL_STATIC_DRAW) {
        std::span _data = data;
        Bind();
        glBufferData(type, _data.size_bytes(), _data.data(), usage);
        last_n_elems = _data.size();
        return *this;
    }

protected:
    void Bind() {
        glBindBuffer(type, handle);
    }
};

class VBO : public BufferObject {
public:
    VBO() : BufferObject(GL_ARRAY_BUFFER) { }
};
class EBO : public BufferObject {
public:
    EBO() : BufferObject(GL_ELEMENT_ARRAY_BUFFER) { }
};



/// Unbinds any VAOs in destructor
class VAO_lock {
public:
    ~VAO_lock() {
        glBindVertexArray(0);
    }
};

class VAO {
    GLuint handle;
public:
    VAO() {
        glGenVertexArrays(1, &handle);
    }
    VAO(const VAO&) = delete;
    VAO(VAO&&) = default;
    ~VAO() {
        glDeleteVertexArrays(1, &handle);
    }

    class Config {
        GLuint vao;
        explicit Config(GLuint vao_handle) : vao(vao_handle) { }
        friend VAO;
    public:
        Config(const Config&) = delete;
        Config(Config&&) = default;

        void Bind(BufferObject& bo) {
            glBindBuffer(bo.type, bo.handle);
        }
    };

    void Configure(const std::function<void(Config)>& config_script) {
        VAO_lock lock;
        Bind(lock);
        config_script(Config(handle));
    }

    /// VAO_lock is just to make it harder to forget
    void Bind(VAO_lock&) {
        glBindVertexArray(handle);
    }
};

class Model {
public:
    VAO vao;
    VBO vbo;
    EBO ebo;

    Model(const std::ranges::range auto& vertices, const std::ranges::range auto& indices) {
        vbo.LoadData(vertices);
        ebo.LoadData(indices);
        vao.Configure([this](VAO::Config c) {
            c.Bind(ebo);
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
        glDrawElements(GL_TRIANGLES, ebo.size(), GL_UNSIGNED_INT, (void*) 0);
    }
};


namespace model_cube {
    constexpr std::array vertices = {
        vertex { { +1.0f, +1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { +1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { -1.0f, +1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        vertex { { +1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { +1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        vertex { { +1.0f, +1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { +1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { -1.0f, +1.0f,  1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        vertex { { +1.0f, +1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { +1.0f, +1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { -1.0f, +1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { -1.0f, +1.0f,  1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        vertex { { +1.0f, +1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { +1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { +1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { +1.0f, +1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        vertex { { -1.0f, +1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        vertex { { -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        vertex { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        vertex { { -1.0f, +1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
    };

    constexpr std::array<unsigned int, 36> indices = {
        0,  1,  2,
        0,  2,  3,
        4,  5,  6,
        4,  6,  7,
        8,  9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23,
    };
}

