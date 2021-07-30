#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <optional>
#include <sstream>
#include <filesystem>

#include <dbg.h>

#include "model_loader.h"
#include "make_shader.h"

#include "BufferObject.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "registerAttributes.h"

struct Mesh {
    VAO vao;
    VBO vbo;
    size_t n_attributes = 0;
};
template <typename T>
Mesh make_mesh(const T& data) {
    VAO vao;
    VBO vbo (data);
    size_t nAttrs;
    {
        vao.Bind();
        vbo.Bind();
        nAttrs = registerAttributes<typename T::value_type>(0);
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
    }
    VAO::Unbind();
    return Mesh { std::move(vao), std::move(vbo), nAttrs };
}

// See MeshEx in assimp.h
inline std::vector<MeshEx> make_model(std::string path) {
    return model_loader::loadModel(std::move(path));
}

/// Does not load data
template <typename T>
VBO make_instance_vbo(auto& mesh, const T& data) {
    int initialAttrs = mesh.n_attributes;
    VBO instance_vbo (data);
    {
        mesh.vao.Bind();
        instance_vbo.Bind();
        size_t nAttrs = registerAttributes<typename T::value_type>(initialAttrs);
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(initialAttrs + i);
            glVertexAttribDivisor(initialAttrs + i, 1);
        }
        mesh.n_attributes += nAttrs;
    }
    VAO::Unbind();
    return instance_vbo;
}

inline Texture make_texture(const char* filename, std::optional<GLenum> format = std::nullopt) {
    file_utils::stbi_data img (filename);
    auto _format = format.value_or(file_utils::stbi_data::resolveChannels(img.nrChannels));
    return Texture ({img.width, img.height, _format, _format, GL_UNSIGNED_BYTE, img.data});
}

/// make_cubemap("textures/skybox/front.png");
inline CubemapTexture make_cubemap(std::filesystem::path path_front, std::optional<GLenum> format = std::nullopt) {
    CubemapTexture ret;
    using direction_t = std::pair<GLenum, std::filesystem::path>;
    std::array<direction_t, 6> directions = {
        direction_t { GL_TEXTURE_CUBE_MAP_POSITIVE_X, "right" },
        direction_t { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "left" },
        direction_t { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "top" },
        direction_t { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "bottom" },
        direction_t { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "back" },
        direction_t { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "front" },
    };
    const auto ext = path_front.extension();
    for (size_t i = 0; i < directions.size(); i++) {
        const std::string filename = path_front.replace_filename(directions[i].second.replace_extension(ext));
        file_utils::stbi_data img (filename.c_str(), false);
        auto _format = format.value_or(file_utils::stbi_data::resolveChannels(img.nrChannels));
        ret.LoadSide(directions[i].first,
                {img.width, img.height, _format, _format, GL_UNSIGNED_BYTE, img.data});
    }
    return ret;
}

inline ShaderProg make_prog(const char* vert_path, const char* frag_path) {
    ShaderProg prog;
    prog.Attach(make_shader(vert_path, GL_VERTEX_SHADER));
    prog.Attach(make_shader(frag_path, GL_FRAGMENT_SHADER));
    link_prog(prog, std::to_array({vert_path, frag_path}));
    return prog;
}
inline ShaderProg make_prog(std::filesystem::path base_path) {
    namespace fs = std::filesystem;
    using fs::path;

    using ext_pair = std::pair<std::string_view, GLuint>;
    static constexpr std::array<ext_pair, 3> exts = {
        std::make_pair ( "vert", GL_VERTEX_SHADER ),
        // std::make_pair ( "tesc", GL_TESS_CONTROL_SHADER ),
        // std::make_pair ( "tese", GL_TESS_EVALUATION_SHADER ),
        std::make_pair ( "geom", GL_GEOMETRY_SHADER ),
        std::make_pair ( "frag", GL_FRAGMENT_SHADER ),
        // std::make_pair ( "comp", GL_COMPUTE_SHADER ),
    };

    ShaderProg prog;
    std::vector<path> found;
    for (const auto& [ext, type] : exts) {
        path f = base_path;
        f.replace_extension(ext);
        if (fs::exists(f)) {
            prog.Attach(make_shader(f, type));
            found.push_back(std::move(f));
        }
    }
    link_prog(prog, found);
    return prog;
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
