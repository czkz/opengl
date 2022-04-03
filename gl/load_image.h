#pragma once
#include "third_party/stb_image/stb_image.h"
#include <glad/glad.h>
#include <string>
#include <stdexcept>

namespace gl::_ {
    inline GLenum channels_to_format (int nrChannels) {
        switch (nrChannels) {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
        }
        throw std::runtime_error(
            "stbi returned unexpected nrChannels: " + std::to_string(nrChannels)
        );
    }
}

namespace gl {
    class image {
    public:
        const int w, h, n_channels;
        const GLenum format;
        unsigned char* data;

    private:
        friend image load_image(const char* filename, bool flip);
        image(int w, int h, int n_channels, GLenum format, unsigned char* data)
            : w(w), h(h), n_channels(n_channels), format(format), data(data) {}
    public:
        image(image&& o)
            : w(o.w), h(o.h), n_channels(o.n_channels), format(o.format), data(o.data) { o.data = nullptr; }
        ~image() { if (data) { stbi_image_free(data); } }
    };

    inline image load_image(const char* filename, bool flip = true) {
        stbi_set_flip_vertically_on_load(flip);
        int w, h, nchan;
        unsigned char* data;
        data = stbi_load(filename, &w, &h, &nchan, 0);
        if (data == nullptr) {
            throw std::runtime_error(
                std::string("Failed to load texture ") + filename
            );
        }
        return image {w, h, nchan, _::channels_to_format(nchan), data};
    }
}
