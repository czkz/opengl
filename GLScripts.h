#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <optional>

#include <dbg.h>

#include "Model.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"

struct Mesh {
    VAO vao;
    size_t n_verts;
};
template <typename T>
Mesh make_vao(const T& data) {
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

ShaderProg make_prog(const char* vert_path, const char* frag_path) {
    ShaderProg prog = [&]() {
        VertexShader v   (file_utils::readFile(vert_path));
        FragmentShader f (file_utils::readFile(frag_path));
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(prog.Link());
    return prog;
}
ShaderProg make_prog(const char* base_path) {
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
FBOStruct make_fbo(int width, int height) {
    Framebuffer fbo;
    auto color_buffer          = Texture (width, height, GL_RGB);
    auto depth_stencil_buffer = Renderbuffer (width, height);

    fbo.Bind();
    color_buffer.AttachToFramebuffer(GL_COLOR_ATTACHMENT0);
    depth_stencil_buffer.AttachToFramebuffer();
    Framebuffer::BindDefault();

    return FBOStruct {
        std::move(fbo),
        std::move(color_buffer),
    };
}
FBOStructEx make_fbo_ex(int width, int height) {
    Framebuffer fbo;
    auto color_buffer = Texture (width, height, GL_RGB);
    auto depth_buffer = Texture (width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

    fbo.Bind();
    color_buffer.AttachToFramebuffer(GL_COLOR_ATTACHMENT0);
    depth_buffer.AttachToFramebuffer(GL_DEPTH_STENCIL_ATTACHMENT);
    Framebuffer::BindDefault();

    return FBOStructEx {
        std::move(fbo),
        std::move(color_buffer),
        std::move(depth_buffer)
    };
}
