#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <functional>
#include "CallbackCapture.h"

class Window : public CallbackCapture<Window> {
    static constexpr int OpenGL_ver_major = 3;
    static constexpr int OpenGL_ver_minor = 3;

    class glfwSystem {
    public:
        glfwSystem () { glfwInit(); }
        ~glfwSystem () { glfwTerminate(); }
    };

public:
    GLFWwindow* handle;

    Window(int width, int height, const char* title, int samples = 1)
        : CallbackCapture(create_window(width, height, title, samples))
        , handle(CallbackCapture::windowHandle)
    {
        registerAllCallbacks();
        MakeContextCurrent();
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

    }
    ~Window() noexcept {
        glfwDestroyWindow(handle);
    }

    /// Set this thread's OpenGL calls to operate on this window.
    /// Done automatically in ctor
    void MakeContextCurrent() {
        glfwMakeContextCurrent(handle);
    }

    std::function<void(float x, float y)> onMouseMove = nullptr;
    std::function<void(int width, int height)> onSizeChanged = nullptr;

private:
    static GLFWwindow* create_window(int width, int height, const char* title, int samples) {
        // Init on first Window, terminate on program termination
        static glfwSystem _glfw = glfwSystem();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, samples);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        GLFWwindow* handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (handle == nullptr) {
            throw std::runtime_error("Failed to create window!");
        }

        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        if (glfwExtensionSupported("WGL_EXT_swap_control_tear") ||
                glfwExtensionSupported("GLX_EXT_swap_control_tear")
        ) {
            dp("***_EXT_swap_control_tear");
            // glfwSwapInterval(-1);  // No vsync
        } else {
            glfwSwapInterval(1);  // Vsync
        }

        return handle;
    }

    void registerAllCallbacks() {
        glfwSetCursorPosCallback(handle, CursorPosCallback);
        glfwSetFramebufferSizeCallback(handle, FramebufferSizeCallback);
    }

private:
    static void FramebufferSizeCallback(GLFWwindow* windowHandle, int width, int height) {
        Window* _this = _these[windowHandle];
        if (_this->onSizeChanged != nullptr) {
            return _this->onSizeChanged(width, height);
        }
    }

    static void CursorPosCallback(GLFWwindow* windowHandle, double x, double y) {
        Window* _this = _these[windowHandle];
        if (_this->onMouseMove != nullptr) {
            return _this->onMouseMove(x, y);
        }
    }
};
