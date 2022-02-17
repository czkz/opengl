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
    GLuint sphere_vao = gl::make_vao(sphere_data, sphere_normals);

    //////// Cube VAO
    std::vector<Vector3> cube_data, cube_normals;
    math::generate_cube(cube_data, cube_normals, 2);
    GLuint cube_vao = gl::make_vao(cube_data, cube_normals);

    //////// Shaders
    GLuint shader_prog = gl::make_shaderprog("shader.vert", "shader.frag");
    GLuint light_shader_prog = gl::make_shaderprog("shader.vert", "light.frag");

    //////// Textures
    GLuint wood_texture = gl::make_texture_srgb(gl::load_image("wood.png"));
    GLuint dev_texture = gl::make_texture(gl::load_image("checkerboard.png"));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wood_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dev_texture);

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
        .position = Vector3(1, 2, 3),
        .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
        .scale = 0.01
    };
    float light_intensity = 1;
    Vector3 light_color { 1, 1, 1 };

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_prog);
        gl::uniform("tex0", 0);
        gl::uniform("tex1", 1);
        gl::uniform("u_time", glfwGetTime());
        gl::uniform("u_camera", camera.Matrix().Inverse());
        gl::uniform("u_camera_world_pos", camera.position);
        gl::uniform("u_projection", projection_matrix);
        gl::uniform("u_light_pos", light.position);
        gl::uniform("u_light_color", light_color);
        gl::uniform("u_light_intensity", light_intensity);
        gl::uniform("u_is_orthographic", isOrthographic);

        // Draw sphere
        gl::uniform("u_transform", sphere_transform.Matrix());
        glBindVertexArray(sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw floor
        gl::uniform("u_transform", floor_transform.Matrix());
        glBindVertexArray(cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw light
        glUseProgram(light_shader_prog);
        gl::uniform("u_camera", camera.Matrix().Inverse());
        gl::uniform("u_transform", light.Matrix());
        gl::uniform("u_projection", projection_matrix);
        gl::uniform("u_light_color", light_color);
        glBindVertexArray(sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

