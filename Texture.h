#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include <stdexcept>
#include <string>
#include <optional>
#include "GLObjectHandles.h"

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
    TextureHandle handle;
    Texture(const char* filename, std::optional<GLenum> format = std::nullopt) {
        this->Bind(GL_TEXTURE_2D);
        stbi_data img (filename);
        auto _format = format.value_or(resolveChannels(img.nrChannels));
        glTexImage2D( GL_TEXTURE_2D, 0, _format,
                img.width, img.height, 0, _format, GL_UNSIGNED_BYTE, img.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /// TODO test if this does anything
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    Texture(GLsizei w, GLsizei h, GLenum format) : Texture(w, h, format, format, GL_UNSIGNED_BYTE) { }
    Texture(GLsizei w, GLsizei h, GLenum internal_format, GLenum format, GLenum type, unsigned char* data = nullptr) {
        this->Bind(GL_TEXTURE_2D);
        glTexImage2D( GL_TEXTURE_2D, 0, internal_format,
                w, h, 0, format, type, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    /// @arg target GL_TEXTURE_2D, etc.
    void Bind(GLenum target) {
        glBindTexture(target, handle.value);
    }

    /// @arg attach_type GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT, etc.
    void AttachToFramebuffer(GLenum attach_type) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attach_type,
                GL_TEXTURE_2D, handle.value, 0);
    }
};
