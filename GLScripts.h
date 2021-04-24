#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <optional>

#include <dbg.h>

#include "Model.h"
#include "Shader.h"
#include "Texture.h"

template <typename T>
std::pair<std::shared_ptr<VAO>, size_t> make_vao(const T& data) {
    auto cubeVAO = std::make_shared<VAO>();
    {
        auto vbo = VBO(data);
        cubeVAO->Bind();
        vbo.Bind();
        size_t nAttrs = T::value_type::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        cubeVAO->Unbind();
    };
    return { cubeVAO, data.size() };
}

ShaderProg make_prog(const char* vert_path, const char* frag_path) {
    ShaderProg prog = [&]() {
        VertexShader v   (vert_path);
        FragmentShader f (frag_path);
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

// struct FboStruct {
//     Framebuffer fbo;
//     Texture color_buffer;
//     std::optional<Texture> depth_stencil_texture;
// };
// __attribute__((always_inline)) inline FboStruct make_fbo(int width, int height) {
//     Framebuffer fbo;
//     Texture texColorBuffer (width, height, GL_RGB);
//     dp(texColorBuffer.handle.value);
//     fbo.Bind();
//     {
//         texColorBuffer.AttachToFramebuffer();
//
//         Renderbuffer rbo (width, height);
//         rbo.AttachToFramebuffer();
//
//         Framebuffer::BindDefault();
//     }
//
//     return FboStruct { std::move(fbo), std::move(texColorBuffer), std::nullopt };
// }
