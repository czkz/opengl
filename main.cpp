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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow*, FPSCamera&, float);
void mouse_callback(GLFWwindow* window, double x, double y);

// settings
const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;

FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };


class glfwHandle {
public:
    glfwHandle() { glfwInit(); }
    ~glfwHandle() { glfwTerminate(); }
};

int main() {
    glfwHandle _glfw;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Sample Text", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSwapInterval(1);

    ShaderProg prog = []() {
        VertexShader v   ("default.vert");
        FragmentShader f ("default.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(prog.Link());

    Model cube (model_cube::vertices, model_cube::indices);

    Texture texture ("container.jpg");
    Texture texture2 ("awesomeface.png");

    float sc = 1;
    FrameCounter frameCounter;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        frameCounter.tick();
        glfwPollEvents();
        processInput(window, camera, frameCounter.deltaTime / 16);

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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClearColor(1., 1., 1., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            // First two commands can be outside the loop
            //  only when there's one shader and one VAO
            prog.Use();
            glBindVertexArray(cube.vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);
        }

        glfwSwapBuffers(window);
    }

    return 0;
}

Vector3 inputMove(GLFWwindow* window) {
    Vector3 v = {0, 0, 0};
    if (glfwGetKey(window, GLFW_KEY_W))          { v.z -= 1; }
    if (glfwGetKey(window, GLFW_KEY_S))          { v.z += 1; }
    if (glfwGetKey(window, GLFW_KEY_A))          { v.x -= 1; }
    if (glfwGetKey(window, GLFW_KEY_D))          { v.x += 1; }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { v.y -= 1; }
    if (glfwGetKey(window, GLFW_KEY_SPACE))      { v.y += 1; }
    if (v) { v.Normalize(); }
    return v;
}

void processInput(GLFWwindow* window, FPSCamera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ENTER)) {
        glfwSetWindowShouldClose(window, true);
    }
    camera.Move(inputMove(window) * deltaTime * 0.05);
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow*, double x, double y) {
    static Vector2 prev (x, y);
    const Vector2 curr (x, y);
    Vector2 v = curr - prev;
    prev = curr;

    constexpr float sensitivity = 0.002;
    v *= sensitivity;

    camera.euler += {v.y, v.x, 0};
    camera.ClampPitch();
    // camera.RotateX(v.y);
    // camera.RotateY(v.x);
}
