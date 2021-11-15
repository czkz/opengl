#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Window.h"

#include "gl/shaders.h"
#include "gl/debug.h"

#include <Vector.h>

#include "util/load_image.h"

#include <Transform.h>

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

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        dp("Debug context active");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl::debug_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }


    constexpr auto tri_data = std::to_array({
        std::make_pair(Vector2{-0.5f, -0.5f}, Vector2{0.0f, 0.0f}),
        std::make_pair(Vector2{+0.5f, -0.5f}, Vector2{1.0f, 0.0f}),
        std::make_pair(Vector2{+0.0f, +0.5f}, Vector2{0.5f, 1.0f}),
    });

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

    //////// VAO, VBO
    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_data), tri_data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

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

    glClearColor(0.1, 0.1, 0.1, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_prog);
        glUniform1i(tex0_location, 0);
        glUniform1i(tex1_location, 1);
        glUniform1f(u_time_location, glfwGetTime());
        {
            float t = glfwGetTime();
            float sint = abs(sin(t));
            Transform transform {
                .position = Vector3(0.5, 0, 0),
                .rotation = Quaternion::Rotation(t, Vector3(0, 0, 1)),
                .scale = sint
            };
            glUniformMatrix4fv(u_transform_location, 1, GL_TRUE, transform.Matrix().data.data());
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

