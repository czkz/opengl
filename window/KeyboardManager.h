#pragma once
#include <functional>
#include <map>
#include <initializer_list>
#include <GLFW/glfw3.h>
#include "CallbackCapture.h"

class KeyboardManager : public CallbackCapture<KeyboardManager> {
public:
    /// @arg key GLFW_KEY_ENTER, GLFW_KEY_W, etc.
    /// @arg isDown true if key was just pressed, false if released
    /// @arg mods bitfield of GLFW_MOD_{SHIFT,CONTROL,ALT,SUPER,CAPS_LOCK,NUM_LOCK}
    using callback_t = std::function<void(int key, bool isDown, int mods)>;

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
    static void key_callback(GLFWwindow* windowHandle, int key, int, int action, int mods) {
        if (action == GLFW_REPEAT) { return; }
        KeyboardManager* _this = _these[windowHandle];
        if (_this->any.contains(key)) {
            _this->any[key](key, action == GLFW_PRESS, mods);
        }
        if (action == GLFW_PRESS && _this->down.contains(key)) {
            _this->down[key](key, action == GLFW_PRESS, mods);
        } else if (action == GLFW_RELEASE && _this->up.contains(key)) {
            _this->up[key](key, action == GLFW_PRESS, mods);
        }
    }
    std::map<int, callback_t> any;
    std::map<int, callback_t> down;
    std::map<int, callback_t> up;
};
