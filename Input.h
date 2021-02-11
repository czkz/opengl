#pragma once
#include <Vector.h>
#include "Camera.h"
#include <GLFW/glfw3.h>

class MoveMaker {
    Vector3 v = {0, 0, 0};
public:
    // void ResetMove() { v = {0, 0, 0}; }
    Vector3 GetMove() { return v ? v.Normalized() : v; }
    void onKeyAny(int key, bool action) {
        int val = action ? 1 : -1;
        switch (key) {
            case GLFW_KEY_W:          { v.z -= val; return; }
            case GLFW_KEY_S:          { v.z += val; return; }
            case GLFW_KEY_A:          { v.x -= val; return; }
            case GLFW_KEY_D:          { v.x += val; return; }
            case GLFW_KEY_LEFT_SHIFT: { v.y -= val; return; }
            case GLFW_KEY_SPACE:      { v.y += val; return; }
        }
    }
};

namespace Input {
    Vector3 inputMove(GLFWwindow* window) {
        Vector3 v = {0, 0, 0};
        if (glfwGetKey(window, GLFW_KEY_S))          { v.y -= 1; }
        if (glfwGetKey(window, GLFW_KEY_W))          { v.y += 1; }
        if (glfwGetKey(window, GLFW_KEY_A))          { v.x -= 1; }
        if (glfwGetKey(window, GLFW_KEY_D))          { v.x += 1; }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { v.z -= 1; }
        if (glfwGetKey(window, GLFW_KEY_SPACE))      { v.z += 1; }
        if (v) { v.Normalize(); }
        return v;
    }
    Vector3 inputRot(GLFWwindow* window) {
        Vector3 v = {0, 0, 0};
        if (glfwGetKey(window, GLFW_KEY_UP))    { v.x += 1; }
        if (glfwGetKey(window, GLFW_KEY_DOWN))  { v.x -= 1; }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) { v.z -= 1; }
        if (glfwGetKey(window, GLFW_KEY_LEFT))  { v.z += 1; }
        if (glfwGetKey(window, GLFW_KEY_Q))     { v.y -= 1; }
        if (glfwGetKey(window, GLFW_KEY_E))     { v.y += 1; }
        if (v) { v.Normalize(); }
        return v;
    }

    namespace Application {
        void onTick(GLFWwindow*) {
        }
    }

    namespace Camera {
        void onMouseMove(class FPSCamera& camera, float x, float y) {
            static Vector2 prev (x, y);
            const Vector2 curr (x, y);
            Vector2 v = curr - prev;
            prev = curr;

            constexpr float sensitivity = 0.002;
            v *= sensitivity;

            camera.euler += {-v.y, 0, -v.x};
            camera.ClampPitch();
        }

        void onTick(GLFWwindow* window, class FPSCamera& camera, float deltaTime) {
            if (const auto move = inputMove(window)) {
                camera.Move(move * deltaTime * 0.05);
            }
            if (const auto rot = inputRot(window)) {
                camera.euler += rot * deltaTime * 0.05;
                camera.ClampPitch();
            }
        }


        void onMouseMove(class SpaceCamera& camera, float x, float y) {
            static Vector2 prev (x, y);
            const Vector2 curr (x, y);
            Vector2 v = curr - prev;
            prev = curr;

            constexpr float sensitivity = 0.002;
            v *= sensitivity;

            camera.RotateX(-v.y);
            camera.RotateZ(-v.x);
        }

        void onTick(GLFWwindow* window, class SpaceCamera& camera, float deltaTime) {
            if (const auto move = inputMove(window)) {
                camera.Move(move * deltaTime * 0.05);
            }
            if (const auto rot = inputRot(window)) {
                camera.rotation = camera.rotation * Quaternion::Euler(rot * deltaTime * 0.05);
            }
        }
    }
}

