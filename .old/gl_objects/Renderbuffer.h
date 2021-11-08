#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include "GLObjectHandles.h"

class Renderbuffer {
    RenderbufferHandle handle;
public:
    Renderbuffer(GLsizei w, GLsizei h) {
        Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    }

    void Bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, handle.value);
    }
    static void Unbind() {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void AttachToFramebuffer() {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  handle.value);
    }
};

