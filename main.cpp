#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Window.h"

#include "gl/shaders.h"
#include "gl/debug.h"

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


    constexpr float tri_data[] = {
        -0.5f, -0.5f,
        +0.5f, -0.5f,
        +0.0f, +0.5f
    };

    GLuint vertex_shader = gl::create_shader(GL_VERTEX_SHADER, "shader.vert");
    GLuint fragment_shader = gl::create_shader(GL_FRAGMENT_SHADER, "shader.frag");

    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);
    glLinkProgram(shader_prog);
    gl::assert_link_successful(shader_prog);

    GLint u_time_location = glGetUniformLocation(shader_prog, "u_time");

    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_data), tri_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);


    glClearColor(0.1, 0.1, 0.1, 1.0);
    while(!glfwWindowShouldClose(window.handle)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_prog);
        glUniform1f(u_time_location, glfwGetTime());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

