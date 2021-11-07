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
    Window window (windowWidth, windowHeight, "Sample Text", 4);
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

    UBO camera_ubo;
    camera_ubo.BindingPoint(0);
    prog.BindUBO("Camera", 0);

    auto model_meshes = make_model("textures/backpack/backpack.obj");
    auto model_instances = VBO();
    for (auto& e : model_meshes) {
        make_mesh_instanced<Transform>(e, model_instances);
    }

    auto floor_mesh = make_mesh(model_cube::vertices);
    auto floor_instances = VBO();
    make_mesh_instanced<Transform>(floor_mesh, floor_instances);
    auto floor_diffuse = make_texture("textures/wood.png");
    auto floor_specular = make_texture("textures/gray.png");

    constexpr Vector3 backgroundColor = {0, 0, 0};
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
    glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        camera_ubo.LoadStruct(UBOStruct::make_camera(camera), GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model_instances.LoadData(std::to_array({
            Transform { {0, 0, 0}, Quaternion::Identity() },
        }), GL_DYNAMIC_DRAW);
        for (auto& e : model_meshes) {
            e.vao.Bind();
            prog.Use();
            prog.SetFloat("u_time", glfwGetTime());
            prog.SetTexture("u_material.diffuse", *e.textures_diffuse.at(0), 0);
            prog.SetTexture("u_material.specular", *e.textures_specular.at(0), 1);
            prog.SetFloat("u_material.shininess", 32);
            glDrawElementsInstanced(GL_TRIANGLES, e.ebo.size(), GL_UNSIGNED_INT, 0, model_instances.size());
        }
        VAO::Unbind();

        floor_instances.LoadData(std::to_array({
            Transform { {0, 0, -20}, Quaternion::Identity(), 30 },
        }), GL_DYNAMIC_DRAW);
        {
            auto& e = floor_mesh;
            e.vao.Bind();
            prog.Use();
            prog.SetFloat("u_time", glfwGetTime());
            prog.SetTexture("u_material.diffuse", floor_diffuse, 0);
            prog.SetTexture("u_material.specular", floor_specular, 1);
            prog.SetFloat("u_material.shininess", 16);
            glDrawArraysInstanced(GL_TRIANGLES, 0, e.vbo.size(), floor_instances.size());
        }
        VAO::Unbind();

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

