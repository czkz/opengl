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

    auto vbo = VBO(model_cube_normals::vertices);
    auto cube_model = std::make_shared<VAO>();
    {
        cube_model->Bind();
        vbo.Bind();
        size_t nAttrs = model_cube_normals::vertex::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        cube_model->Unbind();
    };

    std::vector<Object> cubes;
    for (int i = 0; i < 1000; i++) {
        using namespace std::numbers;
        cubes.emplace_back(
            cube_model,
            Transform {
                Quaternion::Rotation( pi * 2 * phi * i, {0, 0, 1}).Rotate(
                    {(float) pow(i * 2.f, 0.5f), 0, 0.53}
                ),
                Quaternion::Identity(),
                1
            }
        );
    }

    Object light = { cube_model, Transform{ {0, 0, 2}, Quaternion::Identity(), 0.1 } };

    Texture texture ("textures/container2.png");
    Texture textureSpecular ("textures/container2_specular.png");
    Texture textureEmission ("textures/matrix.jpg");

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
        c.SetInt("material.diffuse", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureSpecular.handle);
        c.SetInt("material.specular", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureEmission.handle);
        c.SetInt("material.emission", 2);

        c.SetFloat("material.shininess", 32);

        c.SetVec3("lightPos", light.transform.position);
        c.SetVec3("lightColor", {1, 1, 1});
        c.SetFloat("lightIntensity", (sin(glfwGetTime()) + 1) / 2 * 1);
    };

    Framebuffer fbo;
    fbo.Bind();
    Texture texColorBuffer (windowWidth, windowHeight, GL_RGBA);
    texColorBuffer.AttachToFramebuffer();
    dp(Framebuffer::CheckBoundFrameBuffer());
    Framebuffer::BindDefault();

    VAO screenQuad;
    VBO screenQuadVbo(model_screen_quad::vertices);
    {
        screenQuad.Bind();
        vbo.Bind();
        size_t nAttrs = model_screen_quad::vertex::registerAttributes();
        for (size_t i = 0; i < nAttrs; i++) {
            glEnableVertexAttribArray(i);
        }
        screenQuad.Unbind();
    }

    ShaderProg ppProg = []() {
        VertexShader v   ("postprocessing.vert");
        FragmentShader f ("postprocessing.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(ppProg.Link());


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

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

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(
            backgroundColor.x,
            backgroundColor.y,
            backgroundColor.z,
            1.0
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        {
            prog.UpdateUniformsAndUse();
            cube_model->Bind();
            for (auto& cube : cubes) {
                cube.transform.SetUniforms(prog.GetUniforms());
                glDrawArrays(GL_TRIANGLES, 0, model_cube_normals::vertices.size());
            }
        }

        Framebuffer::BindDefault();
        screenQuad.Bind();
        ppProg.Use();
        glDrawArrays(GL_TRIANGLES, 0, screenQuadVbo.size());

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    VAO::Unbind();

    return 0;
}

