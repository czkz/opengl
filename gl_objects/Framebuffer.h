#pragma once
#include <glad/glad.h>
#include <Vector.h>
#include <span>
#include "GLObjectHandles.h"

class Framebuffer {
    FramebufferHandle handle;
public:
    void Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, handle.value);
    }
    static void BindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    static bool CheckBoundFrameBuffer() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
};

