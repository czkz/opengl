#pragma once
#include <string_view>
#include <functional>
#include <glad/glad.h>
#include <Vector.h>
#include <Quaternion.h>
#include "Texture.h"
#include "file_utils.h"

class Shader {
    static std::string getErrorLog(
        GLuint id,
        std::function<void(GLuint, GLenum, GLint*)> _glGetShaderiv,
        std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> _glGetShaderInfoLog
    ) {
        GLint success;
        _glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success) {
            return "";
        } else {
            GLint logLen;
            _glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
            std::string log (logLen, '\0');
            _glGetShaderInfoLog(id, logLen, nullptr, log.data());
            return log;
        }
    }
private:
    friend class ShaderProg;
    GLuint id = 0;
protected:
    Shader(const char* file_path, GLuint shaderType) {
        id = glCreateShader(shaderType);

        std::string src = file_utils::readFile(file_path);
        const char* srcData = src.data();
        const int srcLen = src.size();
        glShaderSource(id, 1, &srcData, &srcLen);
    }
    Shader(const Shader&) = delete;
    Shader(Shader&& other) {
        id = other.id;
        other.id = 0;
    }
    ~Shader() {
        glDeleteShader(id);
    }
public:
    /// @return Empty string on success
    std::string Compile() {
        glCompileShader(id);
        return getErrorLog(id, glGetShaderiv, glGetShaderInfoLog);
    }
};

class VertexShader : public Shader {
public:
    explicit VertexShader(const char* file_path)
        : Shader(file_path, GL_VERTEX_SHADER) { }
};

class FragmentShader : public Shader {
public:
    explicit FragmentShader(const char* file_path)
        : Shader(file_path, GL_FRAGMENT_SHADER) { }
};

class ShaderProg {
    ShaderProgHandle handle;
public:
    ShaderProg(const VertexShader& v, const FragmentShader& f) {
        glAttachShader(handle.value, v.id);
        glAttachShader(handle.value, f.id);
    }

    std::string Link() {
        glLinkProgram(handle.value);
        return Shader::getErrorLog(handle.value, glGetProgramiv, glGetProgramInfoLog);
    }

    void Use() {
        glUseProgram(handle.value);
    }

public:
    bool SetFloat(const char* name, float value) {
        Use();
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform1f(uniformLocation, value);
        return true;
    }

    bool SetInt(const char* name, int value) {
        Use();
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform1i(uniformLocation, value);
        return true;
    }

    bool SetVec3(const char* name, const Vector3& value) {
        Use();
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform3f(uniformLocation, value.x, value.y, value.z);
        return true;
    }

    bool SetQuaternion(const char* name, const Quaternion& value) {
        Use();
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform4f(uniformLocation, value.v.x, value.v.y, value.v.z, value.s);
        return true;
    }

    bool SetTexture(const char* name, Texture& value,
            int index, GLenum target = GL_TEXTURE_2D)
    {
        Use();
        if (index >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
            return false;
        }
        glActiveTexture(GL_TEXTURE0 + index);
        value.Bind(target);
        if (!SetInt(name, index)) {
            return false;
        }
        return true;
    }
};

