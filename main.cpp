#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Window.h"
#include "util/input/input.h"

#include <Transform.h>
#include <Camera.h>
#include "math/projection.h"

#include "math/gen.h"

#include "gl/debug.h"
#include "gl/shaders.h"
#include "gl/make_vao.h"
#include "gl/make_texture.h"
#include "gl/uniform.h"
#include "gl/handle/handle.h"


int main() try {
    unsigned int windowWidth = 1000;
    unsigned int windowHeight = 1000;
    Window window (windowWidth, windowHeight, "Sample Text", 4, true);
    window.onSizeChanged = [&windowWidth, &windowHeight](int width, int height) {
        glViewport(0, 0, width, height);
        windowWidth = width;
        windowHeight = height;
    };
    KeyboardManager kbManager (window.handle);
    kbManager.onDown(GLFW_KEY_ENTER, [&window](int, bool, int) {
        glfwSetWindowShouldClose(window.handle, true);
    });
    kbManager.onDown(GLFW_KEY_EQUAL, [wireframeEnabled = false](int, bool, int) mutable {
        wireframeEnabled = !wireframeEnabled;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
    });
    // glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gl::enable_debug_context();

    //////// Sphere VAO
    std::vector<Vector3> sphere_data, sphere_normals;
    math::generate_sphere(sphere_data, sphere_normals, 16);
    gl::handle::VAO sphere_vao = gl::make_vao(sphere_data, sphere_normals);

    //////// Cube VAO
    std::vector<Vector3> cube_data, cube_normals;
    math::generate_cube(cube_data, cube_normals, 2);
    gl::handle::VAO cube_vao = gl::make_vao(cube_data, cube_normals);

    //////// Shaders
    gl::handle::ShaderProg shader_prog = gl::make_shaderprog("shader.vert", "shader.frag");
    gl::handle::ShaderProg light_shader_prog = gl::make_shaderprog("shader.vert", "light.frag");
    gl::handle::ShaderProg shadowmap_shader_prog = gl::make_shaderprog("shadowmap.vert", "shadowmap.frag");

    //////// Textures
    gl::handle::Texture wood_texture = gl::make_texture_srgb(gl::load_image("wood.png"));
    gl::handle::Texture dev_texture = gl::make_texture(gl::load_image("checkerboard.png"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, +wood_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, +dev_texture);
    glActiveTexture(GL_TEXTURE0);

    //////// Configure user input
    SpaceCamera camera {{
        .position = Vector3(0, -1, 0),
        .rotation = Quaternion::Identity(),
        .scale = 1
    }};
    bool isOrthographic = false;
    kbManager.onDown(GLFW_KEY_F, [&](int, bool, int) {
        isOrthographic = !isOrthographic;
    });
    window.onMouseMove = [&camera, mouse_prev = Vector2(0, 0)](float x, float y) mutable {
        Vector2 mouse_curr {x, y};
        Vector2 move = mouse_curr - mouse_prev;
        camera.RotateX(-move.y / 50);
        camera.RotateZ(-move.x / 50);
        mouse_prev = mouse_curr;
    };

    //////// Normal objects
    Transform sphere_transform {
        .position = Vector3(0, 0, 0),
        .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
        .scale = 0.5
    };
    Transform floor_transform {
        .position = Vector3(0, 0, -5.75),
        .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
        .scale = 10
    };

    //////// Light
    Transform light {
        .position = Vector3(1, 0, 0),
        .rotation = Quaternion::Rotation(3.1415*0.5, Vector3(0, 0, 1)) * Quaternion::Rotation(-3.1415*0.25, Vector3(1, 0, 0)),
        .scale = 0.01
    };
    float light_intensity = 1;
    Vector3 light_color { 1, 1, 1 };

    //////// Shadows
    constexpr int shadowmap_w = 1024, shadowmap_h = shadowmap_w;
    gl::handle::Texture shadowmap_data;
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, +shadowmap_data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowmap_w, shadowmap_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, std::to_array({ 1.0f, 1.0f, 1.0f }).data());

    gl::handle::Framebuffer shadow_fb;
    glBindFramebuffer(GL_FRAMEBUFFER, +shadow_fb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, +shadowmap_data, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //////// Rendering
    glClearColor(0, 0, 0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    while(!glfwWindowShouldClose(window.handle)) {
        camera.rotation = camera.rotation * Quaternion::Euler(input::get_rotation(window.handle) * 0.05);
        camera.position += camera.rotation.Rotate(input::get_move(window.handle) * 0.05);

        light.position = Vector3(1, 0, (float)(std::sin(glfwGetTime()) + 0.75));

        MatrixS<4, 4> projection_matrix;
        float aspect = float(windowWidth) / windowHeight;
        if (isOrthographic) {
            projection_matrix = math::projection_orthgraphic(5, aspect);
        } else {
            projection_matrix = math::projection_perspective(90, aspect);
        }

        // const MatrixS<4, 4> shadowmap_projection = math::projection_orthgraphic(10, 1);
        const MatrixS<4, 4> shadowmap_projection = math::projection_perspective(90, 1);
        Transform shadow_transform = light;
        shadow_transform.scale = 1;
        const MatrixS<4, 4> shadowmap_camera = shadow_transform.Matrix().Inverse();

        //////// Shadowmap rendering
        glViewport(0, 0, shadowmap_w, shadowmap_h);
        glBindFramebuffer(GL_FRAMEBUFFER, +shadow_fb);
        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(+shadowmap_shader_prog);
        gl::uniform("u_camera", shadowmap_camera);
        gl::uniform("u_projection", shadowmap_projection);

        // Draw sphere
        gl::uniform("u_transform", sphere_transform.Matrix());
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw floor
        gl::uniform("u_transform", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());


        //////// Normal rendering
        glViewport(0, 0, windowWidth, windowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(+shader_prog);
        gl::uniform("tex0", 1);
        gl::uniform("tex1", 2);
        gl::uniform("u_time", glfwGetTime());
        gl::uniform("u_camera", camera.Matrix().Inverse());
        gl::uniform("u_camera_world_pos", camera.position);
        gl::uniform("u_projection", projection_matrix);
        gl::uniform("u_light_pos", light.position);
        gl::uniform("u_light_color", light_color);
        gl::uniform("u_light_intensity", light_intensity);
        gl::uniform("u_is_orthographic", isOrthographic);
        gl::uniform("u_shadowmap", 3);
        gl::uniform("u_shadowmap_camera", shadowmap_camera);
        gl::uniform("u_shadowmap_projection", shadowmap_projection);

        // Draw sphere
        gl::uniform("u_transform", sphere_transform.Matrix());
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw floor
        gl::uniform("u_transform", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw light
        glUseProgram(+light_shader_prog);
        gl::uniform("u_camera", camera.Matrix().Inverse());
        gl::uniform("u_transform", light.Matrix());
        gl::uniform("u_projection", projection_matrix);
        gl::uniform("u_light_color", light_color);
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

