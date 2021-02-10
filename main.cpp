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
// #include "Object.h"

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

    Model cube_model (model_cube::vertices, model_cube::indices);
    // Object cube (cube_model);
    // Model light (model_cube::vertices, model_cube::indices);

    Texture texture ("container.jpg");
    Texture texture2 ("awesomeface.png");

    FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    FrameCounter frameCounter;

    window.onMouseMove = [&camera](float x, float y) {
        Input::FPSCamera::onMouseMove(camera, x, y);
    };

    KeyboardManager kbManager (window.handle);
    kbManager.on(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });

    prog.uniformUpdater = [&camera, &texture, &texture2](ShaderProg::Config c) {
        c.SetFloat("uTime", glfwGetTime());
        c.SetQuaternion("cameraRotation", camera.getRotation());
        c.SetVec3("cameraPosition", camera.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.handle);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.handle);
        c.SetInt("texture1", 0);
        c.SetInt("texture2", 1);
    };

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::FPSCamera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        prog.UpdateUniformsAndUse();

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(1., 1., 1., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            VAO_lock lock;
            // prog.Use();  // Already in UpdateUniformsAndUse()
            cube_model.Draw(lock);
        }

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
}

