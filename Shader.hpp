#pragma once
#include <string_view>
#include <fstream>
#include <iostream>
#include <functional>
#include <glad/glad.h>

class Shader {
    static std::string readFile(const char* path) {
        std::ifstream f(path);
        std::string ret;
        if (!f.good()) {
            return ret;
        }
        f.seekg(0, std::ios::end);
        auto size = f.tellg();
        f.seekg(0, std::ios::beg);
        ret.resize(size, '\0');
        f.read(ret.data(), size);
        return ret;
    }
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
    GLuint id;
protected:
    Shader(const char* file_path, GLuint shaderType) {
        std::string src = readFile(file_path);

        id = glCreateShader(shaderType);

        const char* srcData = src.data();
        const int srcLen = src.size();
        glShaderSource(id, 1, &srcData, &srcLen);
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
    explicit VertexShader(const char* file_path) : Shader(file_path, GL_VERTEX_SHADER) { }
};

class FragmentShader : public Shader {
public:
    explicit FragmentShader(const char* file_path) : Shader(file_path, GL_FRAGMENT_SHADER) { }
};

class ShaderProg {
    GLuint id;
public:
    ShaderProg(const VertexShader& v, const FragmentShader& f) {
        id = glCreateProgram();
        glAttachShader(id, v.id);
        glAttachShader(id, f.id);
    }
    ~ShaderProg() {
        glDeleteProgram(id);
    }

    std::string Link() {
        glLinkProgram(id);
        return Shader::getErrorLog(id, glGetProgramiv, glGetProgramInfoLog);
    }

    void Use() {
        glUseProgram(id);
    }

    bool SetFloat(const char* name, float value) {
        Use();
        GLint uniformLocatuion = glGetUniformLocation(id, name);
        if (uniformLocatuion == -1) {
            return false;
        }
        glUniform1f(uniformLocatuion, value);
        return true;
    }

    bool SetInt(const char* name, int value) {
        Use();
        GLint uniformLocatuion = glGetUniformLocation(id, name);
        if (uniformLocatuion == -1) {
            return false;
        }
        glUniform1i(uniformLocatuion, value);
        return true;
    }
};

