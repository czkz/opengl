#include <string>
#include <glad/glad.h>

std::string getErrorLog(GLuint shader) {
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

std::string getProgErrorLog(GLuint prog) {
    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (success) {
        return "";
    } else {
        GLint logLen;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
        std::string log (logLen, '\0');
        glGetProgramInfoLog(prog, logLen, nullptr, log.data());
        return log;
    }
}
