#pragma once
#include <string_view>
#include <functional>
#include <glad/glad.h>
#include <Vector.h>
#include <Quaternion.h>
#include "Texture.h"
#include "file_utils.h"

class Shader {
    static std::string getErrorLog(GLuint id) {
        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success) {
            return "";
        } else {
            GLint logLen;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
            std::string log (logLen, '\0');
            glGetShaderInfoLog(id, logLen, nullptr, log.data());
            return log;
        }
    }
private:
    friend class ShaderProg;
    ShaderHandle handle;
protected:
    Shader(std::string_view source, GLuint shaderType) : handle(shaderType) {
        const char* srcData = source.data();
        const int srcLen = source.size();
        glShaderSource(handle.value, 1, &srcData, &srcLen);
    }
public:
    /// @return Empty string on success
    std::string Compile() {
        glCompileShader(handle.value);
        return getErrorLog(handle.value);
    }
};

class VertexShader : public Shader {
public:
    explicit VertexShader(std::string_view source)
        : Shader(source, GL_VERTEX_SHADER) { }
};

class FragmentShader : public Shader {
public:
    explicit FragmentShader(std::string_view source)
        : Shader(source, GL_FRAGMENT_SHADER) { }
};

class ShaderProg {
    ShaderProgHandle handle;
public:
    ShaderProg(const VertexShader& v, const FragmentShader& f) {
        glAttachShader(handle.value, v.handle.value);
        glAttachShader(handle.value, f.handle.value);
    }

    std::string Link() {
        glLinkProgram(handle.value);
        return Shader::getErrorLog(handle.value);
    }

    void Use() {
        glUseProgram(handle.value);
    }

// Call Use() before setting uniforms!
public:
    bool SetFloat(const char* name, float value) {
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform1f(uniformLocation, value);
        return true;
    }

    bool SetInt(const char* name, int value) {
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform1i(uniformLocation, value);
        return true;
    }

    bool SetVec3(const char* name, const Vector3& value) {
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform3f(uniformLocation, value.x, value.y, value.z);
        return true;
    }

    bool SetQuaternion(const char* name, const Quaternion& value) {
        GLint uniformLocation = glGetUniformLocation(handle.value, name);
        if (uniformLocation == -1) {
            return false;
        }
        glUniform4f(uniformLocation, value.v.x, value.v.y, value.v.z, value.s);
        return true;
    }

    template <typename T>
    bool SetTexture(const char* name, T& value, int index) {
        if (index >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
            return false;
        }
        glActiveTexture(GL_TEXTURE0 + index);
        value.Bind();
        if (!SetInt(name, index)) {
            return false;
        }
        return true;
    }
};

