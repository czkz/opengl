#include <string>
#include <stdexcept>
#include <glad/glad.h>

#include "_/preprocess_shader.h"

namespace {
    std::string _get_shader_error_log(GLuint shader) {
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
    /// create_shader creates and compiles shader object
    GLuint create_shader(GLenum type, std::filesystem::path file) {
        const std::string src = _::preprocess_shader(file);
        GLuint shader = glCreateShader(type);
        const char* code_data = src.data();
        int code_len = src.length();
        glShaderSource(shader, 1, &code_data, &code_len);
        glCompileShader(shader);
        std::string err = _get_shader_error_log(shader);
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
}

