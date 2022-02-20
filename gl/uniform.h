#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <set>
#include <glad/glad.h>
#include <Vector.h>
#include <Quaternion.h>

namespace gl::_ {

    GLint get_uniform_location(const char* name) {
        constexpr bool soft_unused_uniform_warnings = true;

        GLuint prog; // Non-owning
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &prog);
        if (prog == 0) {
            throw std::runtime_error("Can't set a uniform without an active shader program");
        }
        GLint ret = glGetUniformLocation(prog, name);
        if (ret == -1) {
            std::string msg = std::string("Unused uniform '") + name + "'";
            if (soft_unused_uniform_warnings) {
                static std::set<std::pair<GLuint, std::size_t>> shown_errors;
                if (shown_errors.emplace(prog, std::hash<std::string>{}(msg)).second) {
                    std::cerr << msg << std::endl;
                }
            } else {
                throw std::runtime_error(msg);
            }
        }
        return ret;
    }

}

namespace gl {

    void uniform(const char* name, std::floating_point auto value) {
        glUniform1f(_::get_uniform_location(name), value);
    }

    void uniform(const char* name, int value) {
        glUniform1i(_::get_uniform_location(name), value);
    }

    void uniform(const char* name, unsigned int value) {
        glUniform1ui(_::get_uniform_location(name), value);
    }

    void uniform(const char* name, bool value) {
        glUniform1i(_::get_uniform_location(name), value);
    }

    void uniform(const char* name, const Vector3& value) {
        glUniform3f(_::get_uniform_location(name), value.x, value.y, value.z);
    }

    void uniform(const char* name, const Quaternion& value) {
        glUniform4f(_::get_uniform_location(name), value.v.x, value.v.y, value.v.z, value.s);
    }

    void uniform(const char* name, const MatrixS<4, 4>& value) {
        glUniformMatrix4fv(_::get_uniform_location(name), 1, GL_TRUE, value.data.data());
    }

}
