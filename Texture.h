#pragma once
#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <optional>
#include "file_utils.h"
#include "GLObjectHandles.h"

class Texture {
public:
    TextureHandle handle;
    Texture(GLsizei w, GLsizei h, GLenum format) : Texture(w, h, format, format, GL_UNSIGNED_BYTE) { }
    Texture(GLsizei w, GLsizei h, GLenum internal_format, GLenum format, GLenum type, unsigned char* data = nullptr) {
        this->Bind(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
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
