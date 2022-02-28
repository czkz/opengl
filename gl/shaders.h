#pragma once
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include "handle/handle.h"
#include "_/preprocess_shader.h"

namespace gl::_ {
    std::string get_shader_error_log(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success) {
            return "";
        } else {
            GLint logLen;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            std::string log (logLen, '\0');
            glGetShaderInfoLog(shader, logLen, nullptr, log.data());
            return log;
        }
    }
}

namespace gl {

    /// make_shader creates and compiles shader object
    handle::Shader make_shader(GLenum type, std::filesystem::path file) {
        const std::string src = _::preprocess_shader(file);
        handle::Shader shader { type };
        const char* code_data = src.data();
        int code_len = src.length();
        glShaderSource(+shader, 1, &code_data, &code_len);
        glCompileShader(+shader);
        std::string err = _::get_shader_error_log(+shader);
        if (!err.empty()) {
            throw std::runtime_error(std::string(std::move(file)) + ": " + std::move(err));
        }
        return shader;
    }

    /// assert_link_successful throws if shader program failed to link
    void assert_link_successful(GLuint prog) {
        GLint success;
        glGetProgramiv(prog, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLen;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
            std::string log (logLen, '\0');
            glGetProgramInfoLog(prog, logLen, nullptr, log.data());
            throw std::runtime_error(log);
        }
    }

    // make_shaderprog creates and links a shader program
    handle::ShaderProg make_shaderprog(std::filesystem::path vert, std::filesystem::path frag) {
        handle::Shader vertex_shader = gl::make_shader(GL_VERTEX_SHADER, vert);
        handle::Shader fragment_shader = gl::make_shader(GL_FRAGMENT_SHADER, frag);

        handle::ShaderProg shader_prog;
        glAttachShader(+shader_prog, +vertex_shader);
        glAttachShader(+shader_prog, +fragment_shader);
        glLinkProgram(+shader_prog);
        gl::assert_link_successful(+shader_prog);

        return shader_prog;
    }

    // make_shaderprog creates and links a shader program
    handle::ShaderProg make_shaderprog(std::filesystem::path vert, std::filesystem::path geom, std::filesystem::path frag) {
        handle::Shader vertex_shader = gl::make_shader(GL_VERTEX_SHADER, vert);
        handle::Shader geometry_shader = gl::make_shader(GL_GEOMETRY_SHADER, geom);
        handle::Shader fragment_shader = gl::make_shader(GL_FRAGMENT_SHADER, frag);

        handle::ShaderProg shader_prog;
        glAttachShader(+shader_prog, +vertex_shader);
        glAttachShader(+shader_prog, +geometry_shader);
        glAttachShader(+shader_prog, +fragment_shader);
        glLinkProgram(+shader_prog);
        gl::assert_link_successful(+shader_prog);

        return shader_prog;
    }

}
