#pragma once
#include <glad/glad.h>
#include "handle/handle.h"
#include "load_image.h"

namespace gl {

    struct make_texture_options {
        GLint internalformat = GL_RGB;
        GLenum type = GL_UNSIGNED_BYTE;
        GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;
        GLint mag_filter = GL_LINEAR;
        GLint wrap_s = GL_REPEAT;
        GLint wrap_t = GL_REPEAT;
        bool generate_mipmaps = true;
    };

    /// make_texture creates a texture from file
    handle::Texture make_texture(const gl::image& img, make_texture_options opts = {}) {
        handle::Texture texture;
        glBindTexture(GL_TEXTURE_2D, +texture);
        glTexImage2D(GL_TEXTURE_2D, 0, opts.internalformat, img.w, img.h, 0, img.format, opts.type, img.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.wrap_t);

        if (opts.generate_mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        return texture;
    }

    handle::Texture make_texture_srgb(const gl::image& img, make_texture_options opts = {}) {
        opts.internalformat = GL_SRGB;
        return make_texture(img, std::move(opts));
    }

}
