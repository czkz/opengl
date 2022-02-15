#pragma once
#include <glad/glad.h>
#include "make_buffer.h"

namespace gl {

    /// make_vao creates a VAO and VBOs from arrays of vertex data
    GLuint make_vao(const std::ranges::range auto&... attributes) {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        int i = 0;
        std::array<GLuint, sizeof...(attributes)> vbos { gl::make_buffer(i++, attributes)... };

        glBindVertexArray(0);

        for (auto& vbo : vbos) {
            glDeleteBuffers(1, &vbo);
        }

        return vao;
    }

}
