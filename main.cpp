#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include <numbers>
#include <dbg.h>

#include "GLScripts.h"
#include "model_data.h"
#include "UBOScripts.h"
#include "attributes.h"

#include "Transform.h"
#include "Camera.h"
#include "FrameCounter.h"
#include "Input.h"
#include "KeyboardManager.h"
#include "Window.h"

int main() try {
    constexpr unsigned int windowWidth = 1000;
    constexpr unsigned int windowHeight = 1000;
    Window window (windowWidth, windowHeight, "Sample Text");
    // FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    SpaceCamera camera = { {0, -2000, 0}, Quaternion::Identity() };
    FrameCounter frameCounter;
    float cameraSpeed = 20;
    window.onSizeChanged = [](int width, int height) {
        glViewport(0, 0, width, height);
    };
    window.onMouseMove = [&camera](float x, float y) {
        Input::Camera::onMouseMove(camera, x, y);
    };
    KeyboardManager kbManager (window.handle);
    kbManager.onDown(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });
    kbManager.onDown(GLFW_KEY_9, [&cameraSpeed](int, bool, int) {
        cameraSpeed /= 1.5;
    });
    kbManager.onDown(GLFW_KEY_0, [&cameraSpeed](int, bool, int) {
        cameraSpeed *= 1.5;
    });
    bool wireframeEnabled = false;
    kbManager.onDown(GLFW_KEY_EQUAL, [&wireframeEnabled](int, bool, int) {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
    });

    ShaderProg prog = make_prog("shaders/default");
    auto cube_mesh = make_mesh(model_point::vertices);

    UBO camera_ubo;
    camera_ubo.BindingPoint(0);
    prog.BindUBO("Camera", 0);

    constexpr Vector3 backgroundColor = {0, 0, 0};
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        // dp(frameCounter.deltaTime);
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16, cameraSpeed);

        camera_ubo.LoadStruct(UBOStruct::make_camera(camera), GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto& e = cube_mesh;
            e.vao.Bind();
            prog.Use();
            prog.SetFloat("u_time", glfwGetTime());
            glDrawArraysInstanced(GL_POINTS, 0, e.vbo.size(), 1000000);
        }
        VAO::Unbind();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

