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
    math::mesh sphere = math::generate_sphere(16);
    gl::handle::VAO sphere_vao = gl::make_vao(sphere.pos, sphere.normals, sphere.uvs, sphere.tangents, sphere.bitangents);

    //////// Cube VAO
    math::mesh cube = math::generate_cube(4);
    gl::handle::VAO cube_vao = gl::make_vao(cube.pos, cube.normals, cube.uvs, cube.tangents, cube.bitangents);

    //////// Shaders
    auto shader_prog           = gl::make_shaderprog("shader.vert", "shader.frag");
    auto light_shader_prog     = gl::make_shaderprog("shader.vert", "light.frag");
    auto shadowmap_shader_prog = gl::make_shaderprog("shadow.vert", "shadow.geom", "shadow.frag");
    auto test_shader_prog      = gl::make_shaderprog("cubemap.vert", "cubemap.frag");

    //////// Textures
    gl::handle::Texture bricks_texture = gl::make_texture_srgb(gl::load_image("wood.png"));
    gl::handle::Texture bricks_normal_texture = gl::make_texture(gl::load_image("toy_box_normal.png"));
    gl::handle::Texture bricks_height_texture = gl::make_texture(gl::load_image("toy_box_disp.png"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, +bricks_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, +bricks_normal_texture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, +bricks_height_texture);
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
    glActiveTexture(GL_TEXTURE8);
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

    //////// Postprocessing
    gl::handle::Texture pp_tex_color;
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, +pp_tex_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // gl::handle::Texture pp_tex_depth_stencil;
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, +pp_tex_depth_stencil);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl::handle::Renderbuffer pp_rb_depth_stencil;
    glBindRenderbuffer(GL_RENDERBUFFER, +pp_rb_depth_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, windowWidth, windowHeight);

    gl::handle::Framebuffer pp_fb;
    glBindFramebuffer(GL_FRAMEBUFFER, +pp_fb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, +pp_tex_color, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, +pp_tex_depth_stencil, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, +pp_rb_depth_stencil);

    gl::handle::VAO screen_quad = gl::make_vao(math::screenspace_quad);
    gl::handle::ShaderProg pp_shader_prog = gl::make_shaderprog("pp.vert", "pp.frag");

    //////// Rendering
    glClearColor(0, 0, 0, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

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
        glDrawArrays(GL_TRIANGLES, 0, sphere.pos.size());

        // Draw cube
        gl::uniform("u_M", cube_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube.pos.size());

        // Draw floor
        gl::uniform("u_M", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube.pos.size());


        //////// Normal rendering
        glViewport(0, 0, windowWidth, windowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, +pp_fb);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        glUseProgram(+shader_prog);
        gl::uniform("u_diffuseMap", 1);
        gl::uniform("u_normalMap", 2);
        gl::uniform("u_depthMap", 3);
        gl::uniform("u_time", glfwGetTime());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_light.pos", light.position);
        gl::uniform("u_light.color", light_color);
        gl::uniform("u_light.intensity", light_intensity);
        gl::uniform("u_isOrthographic", isOrthographic);
        gl::uniform("u_shadowmapFarPlane", shadowmap_far_plane);
        gl::uniform("u_shadowmap", 8);

        // Draw sphere
        gl::uniform("u_M", sphere_transform.Matrix());
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere.pos.size());

        // Draw cube
        gl::uniform("u_M", cube_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube.pos.size());

        // Draw floor
        gl::uniform("u_M", floor_transform.Matrix());
        glBindVertexArray(+cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube.pos.size());

        // Draw light
        glUseProgram(+light_shader_prog);
        gl::uniform("u_M", light.Matrix());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_light_color", light_color);
        glBindVertexArray(+sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere.pos.size());

        // Test draw cubemap
        glUseProgram(+test_shader_prog);
        gl::uniform("u_M", Transform { {5, 0, 0}, Quaternion::Identity(), 1 }.Matrix());
        gl::uniform("u_V", math::z_convert * camera.Matrix().Inverse());
        gl::uniform("u_P", projection_matrix);
        gl::uniform("u_shadowmap", 8);
        glBindVertexArray(+cube_vao);
        glDisable(GL_CULL_FACE);
        glDrawArrays(GL_TRIANGLES, 0, cube.pos.size());
        glEnable(GL_CULL_FACE);

        //////// Postprocessing
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glDisable(GL_CULL_FACE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(+pp_shader_prog);
        gl::uniform("u_texture", 9);
        glBindVertexArray(+screen_quad);
        glDrawArrays(GL_TRIANGLES, 0, math::screenspace_quad.size());
        glDisable(GL_FRAMEBUFFER_SRGB);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

