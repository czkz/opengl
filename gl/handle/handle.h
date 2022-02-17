#pragma once
#include <glad/glad.h>
#include <utility>

namespace gl::handle::_ {

    class Handle {
    protected:
        Handle() = default;
        GLuint value = 0;
    public:
        Handle(const Handle&) = delete;
        Handle(Handle&& other) {
            std::swap(value, other.value);
        }
        Handle& operator=(Handle&& other) {
            std::swap(value, other.value);
            return *this;
        }
        GLuint operator +() const {
            return value;
        }
    };

}

namespace gl::handle {

    class Buffer : public _::Handle {
    public:
        Buffer(Buffer&&) = default;
        Buffer& operator=(Buffer&&) = default;
        Buffer() {
            glGenBuffers(1, &value);
        }
        ~Buffer() {
            glDeleteBuffers(1, &value);
        }
    };

    class VAO : public _::Handle {
    public:
        VAO(VAO&&) = default;
        VAO& operator=(VAO&&) = default;
        VAO() {
            glGenVertexArrays(1, &value);
        }
        ~VAO() {
            glDeleteVertexArrays(1, &value);
        }
    };

    class Framebuffer : public _::Handle {
    public:
        Framebuffer(Framebuffer&&) = default;
        Framebuffer& operator=(Framebuffer&&) = default;
        Framebuffer() {
            glGenFramebuffers(1, &value);
        }
        ~Framebuffer() {
            glDeleteFramebuffers(1, &value);
        }
    };

    class Renderbuffer : public _::Handle {
    public:
        Renderbuffer(Renderbuffer&&) = default;
        Renderbuffer& operator=(Renderbuffer&&) = default;
        Renderbuffer() {
            glGenRenderbuffers(1, &value);
        }
        ~Renderbuffer() {
            glDeleteRenderbuffers(1, &value);
        }
    };

    class Texture : public _::Handle {
    public:
        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;
        Texture() {
            glGenTextures(1, &value);
        }
        ~Texture() {
            glDeleteTextures(1, &value);
        }
    };

    class Shader : public _::Handle {
    public:
        Shader(Shader&&) = default;
        Shader& operator=(Shader&&) = default;
        Shader(GLenum shaderType) {
            value = glCreateShader(shaderType);
        }
        ~Shader() {
            glDeleteShader(value);
        }
    };

    class ShaderProg : public _::Handle {
    public:
        ShaderProg(ShaderProg&&) = default;
        ShaderProg& operator=(ShaderProg&&) = default;
        ShaderProg() {
            value = glCreateProgram();
        }
        ~ShaderProg() {
            glDeleteProgram(value);
        }
    };

}
