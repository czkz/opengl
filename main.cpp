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
    FPSCamera camera = { {0, 0, 0}, {0, 0, 0} };
    // SpaceCamera camera = { {0, 0, 0}, Quaternion::Identity() };
    FrameCounter frameCounter;
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
    bool wireframeEnabled = false;
    kbManager.onDown(GLFW_KEY_EQUAL, [&wireframeEnabled](int, bool, int) {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
    });

    ShaderProg prog = make_prog("shaders/default");
    auto cube_mesh = make_mesh(model_cube::vertices);
    auto cube_diffuse = make_texture("textures/container2.png");
    auto cube_specular = make_texture("textures/container2_specular.png");

    VBO cube_instances;
    {
        std::vector<Transform> v;
        for (int i = 0; i < 10000; i++) {
            using namespace std::numbers;
            v.push_back({Quaternion::Rotation(i * 2 * pi * phi, {0, 0, 1}).Rotate({std::sqrt(i*2.0f), 0, 0}), Quaternion::Rotation(i * 2 * pi * phi, {0, 0, 1})});
        }
        cube_instances.LoadData(v, GL_DYNAMIC_DRAW);

        cube_mesh.vao.Bind();
        cube_instances.Bind();
        size_t n = registerAttributes<Transform>(3);
        for (size_t i = 0; i < n; i++) {
            glEnableVertexAttribArray(3+i);
            glVertexAttribDivisor(3+i, 1);
        }

        cube_mesh.vao.Unbind();
    }

    UBO camera_ubo;
    camera_ubo.BindingPoint(0);
    prog.BindUBO("Camera", 0);

    UBO transform_ubo;
    transform_ubo.BindingPoint(1);
    prog.BindUBO("Transform", 1);

    constexpr Vector3 backgroundColor = {0, 0, 0};
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        camera_ubo.LoadStruct(UBOStruct::make_camera(camera), GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto& e = cube_mesh;
            e.vao.Bind();
            prog.Use();
            prog.SetFloat("u_time", glfwGetTime());
            Transform tr {{0, 0, 0}, Quaternion::Rotation(glfwGetTime(), {0, 0, 1})};
            transform_ubo.LoadStruct(UBOStruct::make_transform(tr), GL_DYNAMIC_DRAW);
            prog.SetTexture("u_material.diffuse", cube_diffuse, 0);
            prog.SetTexture("u_material.specular", cube_specular, 1);
            prog.SetFloat("u_material.shininess", 32);
            glDrawArraysInstanced(GL_TRIANGLES, 0, e.vbo.size(), cube_instances.size());
        }
        VAO::Unbind();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

