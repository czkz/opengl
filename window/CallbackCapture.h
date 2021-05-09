#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <dbg.h>

template <typename T>
class CallbackCapture {
protected:
    static std::map<GLFWwindow*, T*> _these;
    GLFWwindow* windowHandle;

    CallbackCapture(GLFWwindow* windowHandle) : windowHandle(windowHandle) {
        if (_these.contains(windowHandle)) {
            throw std::runtime_error("_these already contains this windowHandle");
        }
        _these[windowHandle] = static_cast<T*>(this);
    }
    ~CallbackCapture() {
        _these.erase(windowHandle);
    }
};
template <typename T>
std::map<GLFWwindow*, T*> CallbackCapture<T>::_these;

