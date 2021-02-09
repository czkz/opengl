#pragma once
#include <functional>
#include <map>
#include <initializer_list>
#include <GLFW/glfw3.h>
#include "CallbackCapture.h"

class KeyboardManager : public CallbackCapture<KeyboardManager> {
public:
    using callback_t = std::function<void(int, int, int, int)>;

    KeyboardManager(GLFWwindow* windowHandle) : CallbackCapture(windowHandle) {
        glfwSetKeyCallback(windowHandle, key_callback);
    }

    void on(int key, callback_t handler) {
        any[key] = handler;
    }

    void on(std::initializer_list<int> keys, callback_t handler) {
        for (auto key : keys) {
            any[key] = handler;
        }
    }

private:
    static void key_callback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods) {
        KeyboardManager* _this = _these[windowHandle];
        if (_this->any.contains(key)) {
            _this->any[key](key, scancode, action, mods);
        }
        if (action == GLFW_PRESS && _this->down.contains(key)) {
            _this->down[key](key, scancode, action, mods);
        } else if (action == GLFW_RELEASE && _this->up.contains(key)) {
            _this->up[key](key, scancode, action, mods);
        }
    }
    std::map<int, callback_t> any;
    std::map<int, callback_t> down;
    std::map<int, callback_t> up;
};
