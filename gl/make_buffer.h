#pragma once
#include <span>
#include <stdexcept>
#include <glad/glad.h>
#include <Vector.h>
#include "handle/handle.h"
#include "_/register_attributes.h"

namespace gl {

    /// make_buffer creates and fills a VBO
    handle::Buffer make_buffer(int location, const std::ranges::range auto& data) {
        std::span sp { data };

        handle::Buffer vbo;

        glBindBuffer(GL_ARRAY_BUFFER, +vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            sp.size_bytes(),
            sp.data(),
            GL_STATIC_DRAW
        );

        _::register_attributes<typename decltype(sp)::value_type>(location, true);

        return vbo;
    }

}

