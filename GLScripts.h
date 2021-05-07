#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <optional>
#include <sstream>

#include <dbg.h>

#include "model_loader.h"
#include "BufferObject.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Shader.h"
#include "Texture.h"

struct Mesh {
    VAO vao;
    size_t n_verts;
};
template <typename T>
Mesh make_mesh(const T& data) {
    VAO vao;
    {
        auto vbo = VBO(data);
        vao.Bind();
        vbo.Bind();
        size_t nAttrs = T::value_type::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        VAO::Unbind();
    }
    return { std::move(vao), data.size() };
}

// See MeshEx in assimp.h
inline std::vector<MeshEx> make_model(std::string path) {
    return model_loader::loadModel(std::move(path));
}

inline Texture make_texture(const char* filename, std::optional<GLenum> format = std::nullopt) {
    file_utils::stbi_data img (filename);
    auto _format = format.value_or(file_utils::stbi_data::resolveChannels(img.nrChannels));
    return Texture ({img.width, img.height, _format, _format, GL_UNSIGNED_BYTE, img.data});
}

/// base_path must end with a separator, extension must start with a dot:
/// make_cubemap("textures/skybox/", ".jpg");
inline CubemapTexture make_cubemap(const char* base_path, const char* extension, std::optional<GLenum> format = std::nullopt) {
    CubemapTexture ret;
    std::array<std::pair<GLenum, const char*>, 6> directions = {
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_POSITIVE_X, "right" },
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "left" },
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "top" },
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "bottom" },
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "back" },
        std::pair<GLenum, const char*> { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "front" },
    };
    for (size_t i = 0; i < directions.size(); i++) {
        const std::string filename = std::string(base_path) + directions[i].second + extension;
        file_utils::stbi_data img (filename.c_str(), false);
        auto _format = format.value_or(file_utils::stbi_data::resolveChannels(img.nrChannels));
        ret.LoadSide(directions[i].first,
                {img.width, img.height, _format, _format, GL_UNSIGNED_BYTE, img.data});
    }
    return ret;
}

inline ShaderProg make_prog(const char* vert_path, const char* frag_path) {
    ShaderProg prog = [&]() {
        VertexShader v   (file_utils::readFile(vert_path));
        FragmentShader f (file_utils::readFile(frag_path));
        std::string v_err = v.Compile();
        std::string f_err = f.Compile();
        if (!v_err.empty()) {
            std::ostringstream ss;
            ss << "In " << vert_path << ":\n" << v_err << '\n';
            throw std::runtime_error(ss.str());
        }
        if (!f_err.empty()) {
            std::ostringstream ss;
            ss << "In " << frag_path << ":\n" << f_err << '\n';
            throw std::runtime_error(ss.str());
        }
        return ShaderProg(v, f);
    }();
    std::string prog_err = prog.Link();
    if (!prog_err.empty()) {
        std::ostringstream ss;
        ss << "Linking (" << vert_path << " + " << frag_path << "): " << prog_err << '\n';
        throw std::runtime_error(ss.str());
    }
    return prog;
}
inline ShaderProg make_prog(const char* base_path) {
    return make_prog(
        (base_path + std::string(".vert")).c_str(),
        (base_path + std::string(".frag")).c_str()
    );
}

struct FBOStruct {
    Framebuffer fbo;
    Texture color_buffer;
};
struct FBOStructEx {
    Framebuffer fbo;
    Texture color_buffer;
    Texture depth_stencil_buffer;
};
inline FBOStruct make_fbo(int width, int height) {
    Framebuffer fbo;
    auto color_buffer = Texture (width, height, GL_RGB);
    auto depth_buffer = Renderbuffer (width, height);

    fbo.Bind();
    color_buffer.AttachToFramebuffer(GL_COLOR_ATTACHMENT0);
    depth_buffer.AttachToFramebuffer();
    
    color_buffer.SetWrapType(GL_CLAMP_TO_BORDER);
    color_buffer.SetBorderColor({1, 0, 0});
    Framebuffer::BindDefault();

    return FBOStruct {
        std::move(fbo),
        std::move(color_buffer),
    };
}
inline FBOStructEx make_fbo_ex(int width, int height) {
    Framebuffer fbo;
    auto color_buffer = Texture (width, height, GL_RGB);
    auto depth_buffer = Texture ({width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr});

    fbo.Bind();
    color_buffer.AttachToFramebuffer(GL_COLOR_ATTACHMENT0);
    depth_buffer.AttachToFramebuffer(GL_DEPTH_STENCIL_ATTACHMENT);

    color_buffer.SetWrapType(GL_CLAMP_TO_BORDER);
    color_buffer.SetBorderColor({1, 0, 0});
    Framebuffer::BindDefault();

    return FBOStructEx {
        std::move(fbo),
        std::move(color_buffer),
        std::move(depth_buffer)
    };
}
