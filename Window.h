#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <functional>

class Window {
    static constexpr int OpenGL_ver_major = 3;
    static constexpr int OpenGL_ver_minor = 3;

    class glfwSystem {
    public:
        glfwSystem () { glfwInit(); }
        ~glfwSystem () { glfwTerminate(); }
    };

public:
    GLFWwindow* handle;

    Window(int width, int height, const char* title) {
        // Init on first Window, terminate on program termination
        static glfwSystem _glfw = glfwSystem();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (handle == nullptr) {
            throw std::runtime_error("Failed to create window!");
        }

        glfwSetWindowUserPointer(handle, this);

        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        registerAllCallbacks();
    }
    ~Window() noexcept {
        glfwDestroyWindow(handle);
    }

    /// Set this thread's OpenGL calls to operate on this window
    void MakeContextCurrent() {
        glfwMakeContextCurrent(handle);
    }

    std::function<void(float x, float y)> onMouseMove = nullptr;
    std::function<void(int width, int height)> onSizeChanged = nullptr;

private:
    void registerAllCallbacks() {
        glfwSetCursorPosCallback(handle, CursorPosCallback);
        glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);
    }

private:
    static void FramebufferSizeCallback(GLFWwindow* windowHandle, int width, int height) {
        Window* _this = (Window*) glfwGetWindowUserPointer(windowHandle);
        if (_this->onSizeChanged != nullptr) {
            return _this->onSizeChanged(width, height);
        }
    }

    static void CursorPosCallback(GLFWwindow* windowHandle, double x, double y) {
        Window* _this = (Window*) glfwGetWindowUserPointer(windowHandle);
        if (_this->onMouseMove != nullptr) {
            return _this->onMouseMove(x, y);
        }
    }
};
