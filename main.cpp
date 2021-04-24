#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <Vector.h>
#include <Quaternion.h>
#include <dbg.h>
#include "Camera.h"
#include "FrameCounter.h"
#include "Texture.h"
#include "Model.h"
#include "Window.h"
#include "Input.h"
#include "KeyboardManager.h"
#include "Object.h"
#include <numbers>
#include "model_data.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"

int main() {
    constexpr unsigned int windowWidth = 1000;
    constexpr unsigned int windowHeight = 1000;

    Window window (windowWidth, windowHeight, "Sample Text");
    window.onSizeChanged = [](int width, int height) {
        glViewport(0, 0, width, height);
    };

    ShaderProg prog = []() {
        VertexShader v   ("default.vert");
        FragmentShader f ("default.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(prog.Link());

    auto cubeVAO = std::make_shared<VAO>();
    {
        auto vbo = VBO(model_cube_normals::vertices);
        cubeVAO->Bind();
        vbo.Bind();
        size_t nAttrs = model_cube_normals::vertex::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        cubeVAO->Unbind();
    };

    std::vector<Object> cubes;
    for (int i = 0; i < 1000; i++) {
        using namespace std::numbers;
        cubes.emplace_back(
            cubeVAO,
            Transform {
                Quaternion::Rotation( pi * 2 * phi * i, {0, 0, 1}).Rotate(
                    {(float) pow(i * 2.f, 0.5f), 0, 0.53}
                ),
                Quaternion::Identity(),
                1
            }
        );
    }

    Texture texture ("textures/container2.png");

    FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    // SpaceCamera camera = { {0, 0, 0}, Quaternion::Identity() };

    FrameCounter frameCounter;

    window.onMouseMove = [&camera](float x, float y) {
        Input::Camera::onMouseMove(camera, x, y);
    };

    KeyboardManager kbManager (window.handle);
    kbManager.on(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });

    constexpr Vector3 backgroundColor = {0, 0, 0};

    prog.uniformUpdater = [&](ShaderProg::Uniforms c) {
        c.SetFloat("uTime", glfwGetTime());
        c.SetQuaternion("cameraRotation", camera.getRotation());
        c.SetVec3("cameraPosition", camera.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.handle);
    };

    Framebuffer fbo;
    Texture texColorBuffer (windowWidth, windowHeight, GL_RGB);
    fbo.Bind();
    {
        texColorBuffer.AttachToFramebuffer();

        Renderbuffer rbo (windowWidth, windowHeight);
        rbo.AttachToFramebuffer();

        dp(Framebuffer::CheckBoundFrameBuffer());

        Framebuffer::BindDefault();
    }

    ShaderProg screenShader = []() {
        VertexShader v   ("postprocessing.vert");
        FragmentShader f ("postprocessing.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(screenShader.Link());
    screenShader.uniformUpdater = [&](ShaderProg::Uniforms c) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer.handle);
        c.SetInt("screenTexture", 0);
    };


    auto screenQuadVBO = VBO(model_screen_quad::vertices);
    auto screenQuadVAO = std::make_shared<VAO>();
    {
        screenQuadVAO->Bind();
        screenQuadVBO.Bind();
        size_t nAttrs = model_screen_quad::vertex::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        screenQuadVAO->Unbind();
    }

    glEnable(GL_CULL_FACE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);

    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        for (auto& cube : cubes) {
            cube.transform.rotation = Quaternion::Rotation(
                glfwGetTime() + cube.transform.position.x, {0, 0, 1}
            ) * Quaternion::Rotation(M_PI / 2, {1, 0, 0});
        }

        fbo.Bind();
        {
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            prog.UpdateUniformsAndUse();
            cubeVAO->Bind();
            for (auto& cube : cubes) {
                cube.transform.SetUniforms(prog.GetUniforms());
                glDrawArrays(GL_TRIANGLES, 0, model_cube_normals::vertices.size());
            }
        }

        Framebuffer::BindDefault();
        {
            glDisable(GL_DEPTH_TEST);

            screenShader.UpdateUniformsAndUse();
            screenQuadVAO->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    VAO::Unbind();

    return 0;
}

