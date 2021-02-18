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

// settings
const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;


int main() {
    Window window (1000, 1000, "Sample Text");
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

    ShaderProg lightSourceProg = []() {
        VertexShader v   ("lightsource.vert");
        FragmentShader f ("lightsource.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(lightSourceProg.Link());

    SimpleModel cube_model (model_cube_normals::vertices);
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

    lightSourceProg.uniformUpdater = [&camera, &light](ShaderProg::Uniforms c) {
        c.SetFloat("uTime", glfwGetTime());
        c.SetQuaternion("cameraRotation", camera.getRotation());
        c.SetVec3("cameraPosition", camera.position);

        c.SetVec3("lightPos", light.transform.position);
        c.SetVec3("lightColor", {1, 1, 1});
        // c.SetFloat("lightIntensity", (sin(glfwGetTime()) + 1) / 2 * 1);
        c.SetFloat("lightIntensity", 0);
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

    VAO_lock lock;
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        for (auto& cube : cubes) {
            cube.transform.rotation = Quaternion::Rotation(
                glfwGetTime() + cube.transform.position.x, {0, 0, 1}
            ) * Quaternion::Rotation(M_PI / 2, {1, 0, 0});
        }

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
            for (auto& cube : cubes) {
                cube.transform.SetUniforms(prog.GetUniforms());
                cube.model.Draw(lock);
            }
        }

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
}

