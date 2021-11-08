#include <glad/glad.h>
#include <dbg.h>

#include "KeyboardManager.h"
#include "Window.h"

#include "utils/ShaderParser.h"
#include "scripts.h"

int main() try {
    constexpr unsigned int windowWidth = 1000;
    constexpr unsigned int windowHeight = 1000;
    Window window (windowWidth, windowHeight, "Sample Text", 4);
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


    constexpr float tri_data[] = {
        -0.5f, -0.5f,
        +0.5f, -0.5f,
        +0.0f, +0.5f
    };

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertex_shader_code = parse_shader("shader.vert");
    const char* vertex_shader_code_cstr = vertex_shader_code.c_str();
    int vertex_shader_code_len = vertex_shader_code.length();
    glShaderSource(vertex_shader, 1, &vertex_shader_code_cstr, &vertex_shader_code_len);
    glCompileShader(vertex_shader);
    dp(getErrorLog(vertex_shader));

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragment_shader_code = parse_shader("shader.frag");
    const char* fragment_shader_code_cstr = fragment_shader_code.c_str();
    int fragment_shader_code_len = fragment_shader_code.length();
    glShaderSource(fragment_shader, 1, &fragment_shader_code_cstr, &fragment_shader_code_len);
    glCompileShader(fragment_shader);
    dp(getErrorLog(fragment_shader));

    GLuint shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);
    glLinkProgram(shader_prog);
    dp(getProgErrorLog(shader_prog));

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

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    return 0;
} catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
}

