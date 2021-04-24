#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include <numbers>
#include <dbg.h>

#include "GLScripts.h"
#include "model_data.h"

#include "Transform.h"
#include "Camera.h"
#include "FrameCounter.h"
#include "Input.h"
#include "KeyboardManager.h"
#include "Window.h"

int main() {
    constexpr unsigned int windowWidth = 1000;
    constexpr unsigned int windowHeight = 1000;
    Window window (windowWidth, windowHeight, "Sample Text");
    FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    // SpaceCamera camera = { {0, 0, 0}, Quaternion::Identity() };
    FrameCounter frameCounter;
    window.onSizeChanged = [](int width, int height) {
        glViewport(0, 0, width, height);
    };
    window.onMouseMove = [&camera](float x, float y) {
        Input::Camera::onMouseMove(camera, x, y);
    };
    KeyboardManager kbManager (window.handle);
    kbManager.on(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });

    auto cubeVAO = make_vao(model_cube_normals::vertices);
    std::vector<Transform> cubes;
    for (int i = 0; i < 1000; i++) {
        using namespace std::numbers;
        cubes.emplace_back(
            Quaternion::Rotation( pi * 2 * phi * i, {0, 0, 1} )
                .Rotate( {(float) pow(i * 2.f, 0.5f), 0, 0.53} ),
            Quaternion::Identity(),
            1
        );
    }

    Texture texture ("textures/container2.png");
    ShaderProg prog = make_prog("shaders/default");
    prog.uniformUpdater = [&](ShaderProg::Uniforms c) {
        c.SetFloat("uTime", glfwGetTime());
        c.SetQuaternion("cameraRotation", camera.getRotation());
        c.SetVec3("cameraPosition", camera.position);

        c.SetTexture("texture1", texture, 0, GL_TEXTURE_2D);
        c.SetTexture("texture2", texture, 0, GL_TEXTURE_2D);
    };

    constexpr Vector3 backgroundColor = {0, 0, 0};
    glEnable(GL_CULL_FACE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        prog.UpdateUniformsAndUse();
        cubeVAO.first->Bind();
        for (auto& cube : cubes) {
            cube.rotation =
                Quaternion::Rotation(glfwGetTime() + cube.position.x, {0, 0, 1})
                * Quaternion::Rotation(M_PI / 2, {1, 0, 0});
            cube.SetUniforms(prog.GetUniforms());
            glDrawArrays(GL_TRIANGLES, 0, model_cube_normals::vertices.size());
        }

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
    VAO::Unbind();

    return 0;
}

