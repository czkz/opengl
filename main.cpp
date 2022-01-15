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
    constexpr unsigned int windowWidth = 1000;
    constexpr unsigned int windowHeight = 1000;
    Window window (windowWidth, windowHeight, "Sample Text", 4, true);
    window.onSizeChanged = [](int width, int height) {
        glViewport(0, 0, width, height);
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
    GLint u_projection = glGetUniformLocation(shader_prog, "u_projection");

    //////// VAO, VBO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    std::vector<Vector3> cube_data;
    std::vector<Vector3> cube_normals;
    math::generate_sphere(cube_data, cube_normals, 16);
    GLuint vbo         = util::make_buffer(0, cube_data);
    GLuint vbo_normals = util::make_buffer(1, cube_data);
    (void) vbo;
    (void) vbo_normals;

    //////// Textures
    util::image wood_img = util::load_image("wood.png");
    GLuint wood_texture;
    glGenTextures(1, &wood_texture);
    glBindTexture(GL_TEXTURE_2D, wood_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wood_img.w, wood_img.h, 0, wood_img.format, GL_UNSIGNED_BYTE, wood_img.data);
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

    glClearColor(0.1, 0.1, 0.1, 1.0);
    // glClearColor(0, 0, 0, 1.0);
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window.handle)) {
        camera.rotation = camera.rotation * Quaternion::Euler(input::get_rotation(window.handle) * 0.05);
        camera.position += camera.rotation.Rotate(input::get_move(window.handle) * 0.05);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_prog);
        glUniform1i(tex0_location, 0);
        glUniform1i(tex1_location, 1);
        glUniform1f(u_time_location, glfwGetTime());
        glUniformMatrix4fv(u_camera_location, 1, GL_TRUE, camera.Matrix().Inverse().data.data());
        glUniform3f(u_camera_world_pos_location, camera.position.x, camera.position.y, camera.position.z);
        {
            Transform transform {
                .position = Vector3(0, 0, 0),
                .rotation = Quaternion::Rotation(glfwGetTime(), Vector3(0, 0, 1)),
                .scale = 0.5
            };
            glUniformMatrix4fv(u_transform_location, 1, GL_TRUE, transform.Matrix().data.data());
        }
        glUniformMatrix4fv(u_projection, 1, GL_TRUE, math::projection(90, float(windowWidth) / windowHeight).data.data());
        glDrawArrays(GL_TRIANGLES, 0, cube_data.size());

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

