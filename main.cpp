#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include <numbers>
#include <dbg.h>

#include "GLScripts.h"
#include "model_data.h"
#include "UBOScripts.h"

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
    kbManager.on(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });

    auto cube_mesh = make_mesh(model_cube_textured::vertices);
    std::vector<Transform> cubes;
    for (int i = 0; i < 0; i++) {
        using namespace std::numbers;
        cubes.emplace_back(
            Quaternion::Rotation( pi * 2 * phi * i, {0, 0, 1} )
                .Rotate( {(float) pow(i * 2.f, 0.5f), 0, 0.53} ),
            Quaternion::Identity(),
            1
        );
    }

    auto reflective_prog = make_prog("shaders/reflective");
    auto plane_mesh = make_mesh(model_plane_normals::vertices);
    auto plane_transform = Transform { Vector3 {0, 0, 3}, Quaternion::Identity(), 2 };

    auto postprocessing = make_fbo(windowWidth, windowHeight);
    ShaderProg screenShader = make_prog("shaders/postprocessing");
    auto screenQuad_mesh = make_mesh(model_screen_quad::vertices);

    auto cube_texture = make_texture("textures/container2.png");
    ShaderProg prog = make_prog("shaders/default");

    auto skybox_prog = make_prog("shaders/skybox");
    auto skybox = make_cubemap("textures/skybox/", ".jpg");
    auto skyboxCube_mesh = make_mesh(model_skybox_cube::vertices);

    auto model_meshes = make_model("textures/backpack/backpack.obj");

    UBO camera_ubo;
    camera_ubo.BindingPoint(0);
    prog.BindUBO("CAMERA", 0);
    reflective_prog.BindUBO("CAMERA", 0);
    skybox_prog.BindUBO("CAMERA", 0);

    constexpr Vector3 backgroundColor = {0, 0, 0};
    // glEnable(GL_CULL_FACE);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        frameCounter.tick();
        Input::Application::onTick(window.handle);
        Input::Camera::onTick(window.handle, camera, frameCounter.deltaTime / 16);

        camera_ubo.LoadStruct(UBOStruct::make_camera(camera), GL_DYNAMIC_DRAW);

        postprocessing.fbo.Bind();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.Use();
        prog.SetFloat("uTime", glfwGetTime());
        prog.SetTexture("texture1", cube_texture, 0);
        prog.SetTexture("skybox", skybox, 1);
        cube_mesh.vao.Bind();
        for (auto& cube : cubes) {
            cube.rotation =
                Quaternion::Rotation(glfwGetTime()/10 + cube.position.x, {0, 0, 1})
                * Quaternion::Rotation(std::numbers::pi / 2, {1, 0, 0});
            cube.SetUniforms(prog);
            glDrawArrays(GL_TRIANGLES, 0, cube_mesh.n_verts);
        }
        VAO::Unbind();

        reflective_prog.Use();
        reflective_prog.SetTexture("skybox", skybox, 1);
        plane_mesh.vao.Bind();
        plane_transform.rotation = Quaternion::Rotation(glfwGetTime()/20, {1, 0, 1})
            * Quaternion::Rotation(glfwGetTime()/15, {0, -1, -1});
        plane_transform.SetUniforms(reflective_prog);
        // glDrawArrays(GL_TRIANGLES, 0, plane_mesh.n_verts);
        VAO::Unbind();

        for (auto& e : model_meshes) {
            e.vao.Bind();
            prog.Use();
            Transform {{0, 0, 0}, Quaternion::Identity()}.SetUniforms(prog);
            prog.SetTexture("material.diffuse", *e.textures_diffuse.at(0), 0);
            prog.SetTexture("material.specular", *e.textures_specular.at(0), 1);
            prog.SetFloat("material.shininess", 32);
            glDrawElements(GL_TRIANGLES, e.ebo.size(), GL_UNSIGNED_INT, 0);
        }
        VAO::Unbind();

        // skybox_prog.Use();
        // skybox_prog.SetTexture("skybox", skybox, 0);
        // skyboxCube_mesh.vao.Bind();
        // glDepthFunc(GL_LEQUAL);
        // glDrawArrays(GL_TRIANGLES, 0, skyboxCube_mesh.n_verts);
        // glDepthFunc(GL_LESS);
        // VAO::Unbind();

        Framebuffer::BindDefault();
        glDisable(GL_DEPTH_TEST);
        screenShader.Use();
        screenShader.SetTexture("screenTexture", postprocessing.color_buffer, 0);
        screenQuad_mesh.vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, screenQuad_mesh.n_verts);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

