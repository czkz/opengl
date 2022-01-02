#include <span>
#include <stdexcept>
#include <glad/glad.h>
#include <Vector.h>

#include "_/register_attributes.h"

namespace util {

    /// make_buffer creates and fills a VBO
    GLuint make_buffer(int location, const std::ranges::range auto& data) {
        std::span sp { data };

        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
            GL_ARRAY_BUFFER,
            sp.size_bytes(),
            sp.data(),
            GL_STATIC_DRAW
        );

        util::_::register_attributes<typename decltype(sp)::value_type>(location, true);

        return vbo;
    }

}

