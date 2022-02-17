#pragma once
#include <glad/glad.h>
#include "handle/handle.h"
#include "make_buffer.h"

namespace gl {

    /// make_vao creates a VAO and VBOs from arrays of vertex data
    handle::VAO make_vao(const std::ranges::range auto&... attributes) {
        handle::VAO vao;
        glBindVertexArray(+vao);

        int i = 0;
        std::array<handle::Buffer, sizeof...(attributes)> vbos { gl::make_buffer(i++, attributes)... };

        // VBOs get deleted after this so it's fine
        glBindVertexArray(0);

        return vao;
    }

}
