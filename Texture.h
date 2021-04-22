#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <optional>

class Texture {
    class stbi_data {
    public:
        int width, height, nrChannels;
        unsigned char* data;
        stbi_data(const char* filename) {
                stbi_set_flip_vertically_on_load(true);
                data = stbi_load(filename, &width, &height, &nrChannels, 0);
                if (data == nullptr) {
                    throw std::runtime_error(
                        std::string("Failed to load texture ") + filename
                    );
                }
            }
        ~stbi_data() { stbi_image_free(data); };
    };
    static GLenum resolveChannels(int nrChannels) {
        switch (nrChannels) {
            case 1: return GL_LUMINANCE;
            case 2: return GL_LUMINANCE_ALPHA;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
        }
        throw std::runtime_error(
            "stbi returned unexpected nrChannels: " + std::to_string(nrChannels)
        );
    }
public:
    GLuint handle;
    Texture(const char* filename, std::optional<GLenum> channels = std::nullopt) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_data img (filename);
        auto format = channels.value_or(resolveChannels(img.nrChannels));
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            img.width,
            img.height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            img.data
        );
        /// TODO test if this does anything
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    Texture(GLsizei w, GLsizei h, GLenum channels) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            channels,
            w,
            h,
            0,
            channels,
            GL_UNSIGNED_BYTE,
            nullptr
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    ~Texture() {
        glDeleteTextures(1, &handle);
    }

    void AttachToFramebuffer() {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
    }
};
