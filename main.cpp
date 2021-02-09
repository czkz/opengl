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

// settings
const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;

int main() {
    Window window (1000, 1000, "Sample Text");
    window.MakeContextCurrent();
    window.onSizeChanged = [](int width, int height) {
        glViewport(0, 0, width, height);
    };

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSwapInterval(1);

    ShaderProg prog = []() {
        VertexShader v   ("fog.vert");
        FragmentShader f ("fog.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(prog.Link());

    Model cube (model_cube::vertices, model_cube::indices);

    Texture texture ("container.jpg");
    Texture texture2 ("awesomeface.png");

    float sc = 1;
    FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    FrameCounter frameCounter;

    window.onMouseMove = [&camera](float x, float y) {
        Input::FPSCamera::onMouseMove(camera, x, y);
    };

    KeyboardManager kbManager (window.handle);
    kbManager.on(GLFW_KEY_ENTER, [&window](int, int, int, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        glfwPollEvents();
        Input::Application::onTick(window.handle);
        Input::FPSCamera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        prog.SetFloat("uTime", glfwGetTime());
        prog.SetQuaternion("q", camera.getRotation());
        prog.SetVec3("tr", camera.position);
        prog.SetFloat("sc", sc);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.handle);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.handle);
        prog.SetInt("texture1", 0);
        prog.SetInt("texture2", 1);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(1., 1., 1., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            // First two commands can be outside the loop
            //  only when there's one shader and one VAO
            prog.Use();
            glBindVertexArray(cube.vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);
        }

        glfwSwapBuffers(window.handle);
    }

    return 0;
}

