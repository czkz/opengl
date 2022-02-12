#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Window.h"

#include "gl/shaders.h"
#include "gl/debug.h"

#include <Vector.h>

#include "util/load_image.h"

#include <Transform.h>

#include <Camera.h>
#include "util/input/input.h"
#include "util/math/projection.h"

#include "util/math/gen.h"

#include "util/make_buffer.h"


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

    //////// Shaders
    GLuint vertex_shader = gl::create_shader(GL_VERTEX_SHADER, "shader.vert");
    GLuint fragment_shader = gl::create_shader(GL_FRAGMENT_SHADER, "shader.frag");

    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);
    glLinkProgram(shader_prog);
    gl::assert_link_successful(shader_prog);

    GLint u_time_location = glGetUniformLocation(shader_prog, "u_time");
    GLint tex0_location = glGetUniformLocation(shader_prog, "tex0");
    GLint tex1_location = glGetUniformLocation(shader_prog, "tex1");
    GLint u_transform_location = glGetUniformLocation(shader_prog, "u_transform");
    GLint u_camera_location = glGetUniformLocation(shader_prog, "u_camera");
    GLint u_camera_world_pos_location = glGetUniformLocation(shader_prog, "u_camera_world_pos");
    GLint u_projection_location = glGetUniformLocation(shader_prog, "u_projection");
    GLint u_light_pos_location = glGetUniformLocation(shader_prog, "u_light_pos");
    GLint u_light_color_location = glGetUniformLocation(shader_prog, "u_light_color");

    //////// Sphere VAO, VBO
    GLuint sphere_vao;
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);

    std::vector<Vector3> sphere_data;
    std::vector<Vector3> sphere_normals;
    math::generate_sphere(sphere_data, sphere_normals, 16);
    GLuint sphere_vbo         = util::make_buffer(0, sphere_data);
    GLuint sphere_vbo_normals = util::make_buffer(1, sphere_normals);
    (void) sphere_vbo;
    (void) sphere_vbo_normals;

    //////// Cube VAO, VBO
    GLuint cube_vao;
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    std::vector<Vector3> cube_data;
    std::vector<Vector3> cube_normals;
    math::generate_cube(cube_data, cube_normals, 2);
    GLuint cube_vbo         = util::make_buffer(0, cube_data);
    GLuint cube_vbo_normals = util::make_buffer(1, cube_normals);
    (void) cube_vbo;
    (void) cube_vbo_normals;

    //////// Textures
    util::image wood_img = util::load_image("wood.png");
    GLuint wood_texture;
    glGenTextures(1, &wood_texture);
    glBindTexture(GL_TEXTURE_2D, wood_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, wood_img.w, wood_img.h, 0, wood_img.format, GL_UNSIGNED_BYTE, wood_img.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    util::image dev_img = util::load_image("checkerboard.png");
    GLuint dev_texture;
    glGenTextures(1, &dev_texture);
    glBindTexture(GL_TEXTURE_2D, dev_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dev_img.w, dev_img.h, 0, dev_img.format, GL_UNSIGNED_BYTE, dev_img.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //////// Bind textures
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
    { // Mouse rotation
        Vector2 mouse_prev {0, 0};
        window.onMouseMove = [&camera, mouse_prev](float x, float y) mutable {
            Vector2 mouse_curr {x, y};
            Vector2 move = mouse_curr - mouse_prev;
            camera.RotateX(-move.y / 50);
            camera.RotateZ(-move.x / 50);
            mouse_prev = mouse_curr;
        };
    }

    //////// Light
    Transform light {
        .position = Vector3(1, 2, 3),
        .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
        .scale = 0.1
    };
    Vector3 light_color { 1, 1, 1 };

    //////// Light shader
    GLuint light_vertex_shader = gl::create_shader(GL_VERTEX_SHADER, "shader.vert");
    GLuint light_fragment_shader = gl::create_shader(GL_FRAGMENT_SHADER, "light.frag");

    GLuint light_shader_prog = glCreateProgram();
    glAttachShader(light_shader_prog, light_vertex_shader);
    glAttachShader(light_shader_prog, light_fragment_shader);
    glLinkProgram(light_shader_prog);
    gl::assert_link_successful(light_shader_prog);

    GLint light_u_transform_location = glGetUniformLocation(light_shader_prog, "u_transform");
    GLint light_u_camera_location = glGetUniformLocation(light_shader_prog, "u_camera");
    GLint light_u_projection_location = glGetUniformLocation(light_shader_prog, "u_projection");
    GLint light_u_light_color_location = glGetUniformLocation(light_shader_prog, "u_light_color");

    //////// Rendering
    glClearColor(0, 0, 0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    while(!glfwWindowShouldClose(window.handle)) {
        camera.rotation = camera.rotation * Quaternion::Euler(input::get_rotation(window.handle) * 0.05);
        camera.position += camera.rotation.Rotate(input::get_move(window.handle) * 0.05);

        light.position = Quaternion::Rotation(glfwGetTime(), {0, 0, 1}).Rotate({1, 0, 3});

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_prog);
        glUniform1i(tex0_location, 0);
        glUniform1i(tex1_location, 1);
        glUniform1f(u_time_location, glfwGetTime());
        glUniformMatrix4fv(u_camera_location, 1, GL_TRUE, camera.Matrix().Inverse().data.data());
        glUniform3f(u_camera_world_pos_location, camera.position.x, camera.position.y, camera.position.z);
        glUniformMatrix4fv(u_projection_location, 1, GL_TRUE, math::projection(90, float(windowWidth) / windowHeight).data.data());
        glUniform3f(u_light_pos_location, light.position.x, light.position.y, light.position.z);
        glUniform3f(u_light_color_location, light_color.x, light_color.y, light_color.z);

        // Draw sphere
        {
            Transform transform {
                .position = Vector3(0, 0, 0),
                .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
                .scale = 0.5
            };
            glUniformMatrix4fv(u_transform_location, 1, GL_TRUE, transform.Matrix().data.data());
        }
        glBindVertexArray(sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        // Draw floor
        {
            Transform transform {
                .position = Vector3(0, 0, -5.75),
                .rotation = Quaternion::Rotation(0, Vector3(0, 0, 1)),
                .scale = 10
            };
            glUniformMatrix4fv(u_transform_location, 1, GL_TRUE, transform.Matrix().data.data());
        }
        glBindVertexArray(cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        // Draw light
        glUseProgram(light_shader_prog);
        glUniformMatrix4fv(light_u_camera_location, 1, GL_TRUE, camera.Matrix().Inverse().data.data());
        glUniformMatrix4fv(light_u_transform_location, 1, GL_TRUE, light.Matrix().data.data());
        glUniformMatrix4fv(light_u_projection_location, 1, GL_TRUE, math::projection(90, float(windowWidth) / windowHeight).data.data());
        glUniform3f(light_u_light_color_location, light_color.x, light_color.y, light_color.z);
        glBindVertexArray(sphere_vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere_data.size());

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

