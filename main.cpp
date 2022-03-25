#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Matrix.h"
#include "Vector.h"
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

using std::numbers::pi;

void generate_TB(
    std::ranges::input_range auto&& vertex_pos,
    std::ranges::input_range auto&& vertex_uv,
    std::vector<Vector3>& vertex_tangent,
    std::vector<Vector3>& vertex_bitangent
) {
    vertex_tangent.reserve(std::size(vertex_pos));
    vertex_bitangent.reserve(std::size(vertex_pos));
    for (size_t i = 0; i < std::size(vertex_pos); i += 3) {
        const std::span v = std::span(vertex_pos).subspan(i, 3);
        const std::span uv = std::span(vertex_uv).subspan(i, 3);
        const MatrixS<3, 2> dw ({
            (v[1]-v[0]).x, (v[2]-v[0]).x,
            (v[1]-v[0]).y, (v[2]-v[0]).y,
            (v[1]-v[0]).z, (v[2]-v[0]).z,
        });
        const MatrixS<2, 2> duv ({
            (uv[1]-uv[0]).x, (uv[2]-uv[0]).x,
            (uv[1]-uv[0]).y, (uv[2]-uv[0]).y,
        });
        const MatrixS<3, 2> TB = dw * duv.Inverse();
        for (size_t i = 0; i < 3; i++) {
            vertex_tangent.emplace_back(
                TB(0, 0),
                TB(1, 0),
                TB(2, 0)
            );
            vertex_bitangent.emplace_back(
                TB(0, 1),
                TB(1, 1),
                TB(2, 1)
            );
        }
    }
}

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
    std::vector<Vector3> sphere_data, sphere_normals, sphere_tangents, sphere_bitangents;
    std::vector<Vector2> sphere_uvs;
    math::generate_sphere(sphere_data, sphere_normals, 16);
    math::_::generate_sphere_uvs(sphere_data, sphere_uvs);
    generate_TB(sphere_data, sphere_uvs, sphere_tangents, sphere_bitangents);
    gl::handle::VAO sphere_vao = gl::make_vao(sphere_data, sphere_normals, sphere_uvs, sphere_tangents, sphere_bitangents);

    //////// Cube VAO
    std::vector<Vector3> cube_data, cube_normals, cube_tangents, cube_bitangents;
    std::vector<Vector2> cube_uvs;
    math::_::generate_cube_uvs(cube_uvs, 4);
    math::generate_cube(cube_data, cube_normals, 4);
    generate_TB(cube_data, cube_uvs, cube_tangents, cube_bitangents);
    gl::handle::VAO cube_vao = gl::make_vao(cube_data, cube_normals, cube_uvs, cube_tangents, cube_bitangents);

    //////// Shaders
    gl::handle::ShaderProg shader_prog = gl::make_shaderprog("shader.vert", "shader.frag");
    gl::handle::ShaderProg light_shader_prog = gl::make_shaderprog("shader.vert", "light.frag");
    gl::handle::ShaderProg shadowmap_shader_prog = gl::make_shaderprog("shadow.vert", "shadow.geom", "shadow.frag");
    gl::handle::ShaderProg test_shader_prog = gl::make_shaderprog("cubemap.vert", "cubemap.frag");

    //////// Textures
    // gl::handle::Texture wood_texture = gl::make_texture_srgb(gl::load_image("wood.png"));
    // gl::handle::Texture dev_texture = gl::make_texture(gl::load_image("checkerboard.png"));
    gl::handle::Texture bricks_texture = gl::make_texture(gl::load_image("bricks.jpg"));
    gl::handle::Texture bricks_normal_texture = gl::make_texture(gl::load_image("bricks_normal.jpg"));
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, +wood_texture);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, +dev_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, +bricks_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, +bricks_normal_texture);
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
        .position = Vector3(0, 0.5, -0.5),
        .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
        .scale = 0.5
    };
    Transform cube_transform {
        .position = Vector3(0, -0.5, -0.5),
        .rotation = Quaternion::Rotation(1, Vector3(1, 1, 1)),
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
        .rotation = Quaternion::Rotation(3.1415*0.5, Vector3(0, 0, 1)),
        .scale = 0.01
    };
    float light_intensity = 1;
    Vector3 light_color { 1, 1, 1 };

    //////// Shadows
    constexpr int shadowmap_w = 1024, shadowmap_h = shadowmap_w;
    constexpr float shadowmap_far_plane = 100;
    gl::handle::Texture shadowmap_data;
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, +shadowmap_data);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowmap_w, shadowmap_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl::handle::Framebuffer shadow_fb;
    glBindFramebuffer(GL_FRAMEBUFFER, +shadow_fb);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, +shadowmap_data, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //////// Rendering
    glClearColor(0, 0, 0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    while(!glfwWindowShouldClose(window.handle)) {
        camera.rotation = camera.rotation * Quaternion::Euler(input::get_rotation(window.handle) * 0.05);
        camera.position += camera.rotation.Rotate(input::get_move(window.handle) * 0.01);

        // light.position = Vector3(1, 0, (float)(std::sin(glfwGetTime()) + 0.75));

        MatrixS<4, 4> projection_matrix;
        float aspect = float(windowWidth) / windowHeight;
        if (isOrthographic) {
            projection_matrix = math::projection_orthgraphic(5, aspect);
        } else {
            projection_matrix = math::projection_perspective(90, aspect);
        }

        //////// Shadowmap rendering
        glViewport(0, 0, shadowmap_w, shadowmap_h);
        glBindFramebuffer(GL_FRAMEBUFFER, +shadow_fb);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);

        glUseProgram(+shadowmap_shader_prog);
        gl::uniform("u_V[0]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 1.5, {0, 0, 1}) * Quaternion::Rotation(pi * 0.5, {0, 1, 0}), 1 }.Matrix().Inverse()); // +X
        gl::uniform("u_V[1]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 0.5, {0, 0, 1}) * Quaternion::Rotation(pi * 1.5, {0, 1, 0}), 1 }.Matrix().Inverse()); // -X
        gl::uniform("u_V[2]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 0.0, {0, 0, 1}) * Quaternion::Rotation(pi * 0.0, {0, 1, 0}), 1 }.Matrix().Inverse()); // +Y
        gl::uniform("u_V[3]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 1.0, {0, 0, 1}) * Quaternion::Rotation(pi * 1.0, {0, 1, 0}), 1 }.Matrix().Inverse()); // -Y
        gl::uniform("u_V[4]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 0.5, {1, 0, 0}) * Quaternion::Rotation(pi * 0.0, {0, 1, 0}), 1 }.Matrix().Inverse()); // +Z
        gl::uniform("u_V[5]", math::z_convert * Transform { light.position, Quaternion::Rotation(pi * 1.5, {1, 0, 0}) * Quaternion::Rotation(pi * 1.0, {0, 1, 0}), 1 }.Matrix().Inverse()); // -Z
        gl::uniform("u_P", math::projection_perspective(90, 1));
        gl::uniform("u_light.pos", light.position);
        gl::uniform("u_shadowmapFarPlane", shadowmap_far_plane);

        // Draw sphere
        gl::uniform("u_M", sphere_transform.Matrix());
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw cube
        gl::uniform("u_M", cube_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw floor
        gl::uniform("u_M", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());


        //////// Normal rendering
        glViewport(0, 0, windowWidth, windowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        glUseProgram(+shader_prog);
        gl::uniform("u_tex0", 1);
        gl::uniform("u_tex1", 2);
        gl::uniform("u_time", glfwGetTime());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_cameraPosW", camera.position);
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_light.pos", light.position);
        gl::uniform("u_light.color", light_color);
        gl::uniform("u_light.intensity", light_intensity);
        gl::uniform("u_isOrthographic", isOrthographic);
        gl::uniform("u_shadowmapFarPlane", shadowmap_far_plane);
        gl::uniform("u_shadowmap", 3);

        // Draw sphere
        gl::uniform("u_M", sphere_transform.Matrix());
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw cube
        gl::uniform("u_M", cube_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw floor
        gl::uniform("u_M", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw light
        glUseProgram(+light_shader_prog);
        gl::uniform("u_M", light.Matrix());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_light_color", light_color);
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Test draw cubemap
        glUseProgram(+test_shader_prog);
        gl::uniform("u_M", Transform { {5, 0, 0}, Quaternion::Identity(), 1 }.Matrix());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_shadowmap", 3);
        glBindVertexArray(+cube_vao);
        glDisable(GL_CULL_FACE);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());
        glEnable(GL_CULL_FACE);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

