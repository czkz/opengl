#pragma once
#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <optional>
#include <array>
#include "file_utils.h"
#include "GLObjectHandles.h"

class Texture {
public:
    struct ImageInfo {
        GLsizei w;
        GLsizei h;
        GLenum internal_format;
        GLenum format;
        GLenum type;
        unsigned char* data;
    };

public:
    static constexpr GLenum texture_target = GL_TEXTURE_2D;
    TextureHandle handle;

    Texture(GLsizei w, GLsizei h, GLenum format)
        : Texture({w, h, format, format, GL_UNSIGNED_BYTE, nullptr}) { }
    Texture(const ImageInfo& image) {
        this->Bind();
        glTexImage2D(texture_target, 0, image.internal_format,
                image.w, image.h, 0, image.format, image.type, image.data);
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void Bind() {
        glBindTexture(texture_target, handle.value);
    }

    /// Default type is GL_REPEAT
    /// @arg type GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT, etc.
    void SetWrapType(GLenum type) {
        this->Bind();
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, type);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, type);
    }

    void SetBorderColor(std::array<float, 3> rgb) {
        glTexParameterfv(texture_target, GL_TEXTURE_BORDER_COLOR, rgb.data());
    }

    /// @arg attach_type GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT, etc.
    void AttachToFramebuffer(GLenum attach_type) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attach_type,
                texture_target, handle.value, 0);
    }
};

class CubemapTexture {
public:
    static constexpr GLenum texture_target = GL_TEXTURE_CUBE_MAP;
    TextureHandle handle;
    CubemapTexture() {
        this->Bind();
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    /// @arg side GL_TEXTURE_CUBE_MAP_POSITIVE_X, etc.
    void LoadSide(GLenum side, const Texture::ImageInfo& image) {
        this->Bind();
        glTexImage2D(side, 0, image.internal_format,
                image.w, image.h, 0, image.format, image.type, image.data);
    }

    void Bind() {
        glBindTexture(texture_target, handle.value);
    }

    /// @arg attach_type GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT, etc.
    void AttachToFramebuffer(GLenum attach_type) {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               attach_type,
                               GL_TEXTURE_2D,
                               handle.value,
                               0);
    }
};
