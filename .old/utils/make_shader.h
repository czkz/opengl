#pragma once
#include "Shader.h"
#include "ShaderParser.h"
#include <filesystem>

inline Shader make_shader(std::filesystem::path path,
                          GLuint type) {
    Shader s (ShaderParser().parse(path), type);
    std::string err = s.Compile();
    if (!err.empty()) {
        std::ostringstream ss;
        ss << "In " << path << ":\n" << err << '\n';
        throw std::runtime_error(ss.str());
    }
    return s;
}

inline void link_prog(ShaderProg& prog,
                      const auto& shader_paths)
{
    std::string err = prog.Link();
    if (!err.empty()) {
        std::ostringstream ss;
        ss << "Linking (";
        for (size_t i = 0; i < shader_paths.size(); i++) {
            ss << shader_paths[i];
            if (i != shader_paths.size() - 1) {
                ss << " + ";
            }
        }
        ss << "): " << err << '\n';
        throw std::runtime_error(ss.str());
    }
}
