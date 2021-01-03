#pragma once
#include <string_view>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

class ShaderProg {
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
    private:
        GLuint id;
    private:
        friend ShaderProg;
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

        /// @return Empty string on success
        std::string Compile() {
            glCompileShader(id);
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
    };
public:
    GLuint id;
    
    ShaderProg(const char* vert_path, const char* frag_path) {
        Shader v (vert_path, GL_VERTEX_SHADER);
        Shader f (frag_path, GL_FRAGMENT_SHADER);
        std::string l1 = v.Compile();
        std::string l2 = f.Compile();
        if (!l1.empty()) {
            std::cout << l1 << std::endl;
        }
        if (!l2.empty()) {
            std::cout << l2 << std::endl;
        }

        id = glCreateProgram();
        glAttachShader(id, v.id);
        glAttachShader(id, f.id);

    }
    ~ShaderProg() {
        glDeleteProgram(id);
    }

    std::string Link() {
        glLinkProgram(id);

        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (success) {
            return "";
        } else {
            GLint logLen;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);
            std::string log (logLen, '\0');
            glGetProgramInfoLog(id, logLen, nullptr, log.data());
            return log;
        }
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
};

