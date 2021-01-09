#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <Shader.h>
#include <Vector.h>
#include <Quaternion.h>
#include <dbg.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;


class glfwHandle {
public:
    glfwHandle() { glfwInit(); }
    ~glfwHandle() { glfwTerminate(); }
};

int main() {
    glfwHandle _glfw;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Sample Text", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSwapInterval(1);

    ShaderProg prog = []() {
        VertexShader v   ("default.vert");
        FragmentShader f ("default.frag");
        dp(v.Compile());
        dp(f.Compile());
        return ShaderProg(v, f);
    }();
    dp(prog.Link());

    GLuint vao;
    GLuint vbo;
    GLuint ubo;
    {
        float vertices[] = {
            // positions          // colors           // texture coords
            1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
           -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

            1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
           -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

            1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
           -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

            1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           -1.0f,  1.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
           -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

            1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
            1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

           -1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
           -1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
           -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        };

        unsigned int indices[] = {
             0,  1,  2,
             0,  2,  3,
             4,  5,  6,
             4,  6,  7,
             8,  9, 10,
             8, 10, 11,
            12, 13, 14,
            12, 14, 15,
            16, 17, 18,
            16, 18, 19,
            20, 21, 22,
            20, 22, 23,
        };

        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        glGenBuffers(1, &ubo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0                   );
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)) );
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)) );
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    GLuint texture;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        {
            stbi_set_flip_vertically_on_load(true);
            int width, height, nrChannels;
            unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                // glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                std::cout << "Failed to load texture" << std::endl;
            }

            stbi_image_free(data);
        }
    }

    GLuint texture2;
    {
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        {
            stbi_set_flip_vertically_on_load(true);
            int width, height, nrChannels;
            unsigned char* data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                std::cout << "Failed to load texture" << std::endl;
            }

            stbi_image_free(data);
        }
    }

    Quaternion q = Quaternion::Identity();
    Vector3 tr = {0, 0, 0};
    float sc = 1;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        constexpr float r = 1;
        constexpr float m = 0.05;
        if (glfwGetKey(window, GLFW_KEY_E)) {
            q = Quaternion::Rotation(+r * M_PI/180.f, {0, 0, 1}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_Q)) {
            q = Quaternion::Rotation(-r * M_PI/180.f, {0, 0, 1}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_UP)) {
            q = Quaternion::Rotation(-r * M_PI/180.f, {1, 0, 0}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            q = Quaternion::Rotation(+r * M_PI/180.f, {1, 0, 0}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            q = Quaternion::Rotation(-r * M_PI/180.f, {0, 1, 0}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            q = Quaternion::Rotation(+r * M_PI/180.f, {0, 1, 0}) * q;
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            tr += q.Inverse().Rotate({0, 0, m});
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            tr -= q.Inverse().Rotate({0, 0, m});
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            tr += q.Inverse().Rotate({m, 0, 0});
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            tr -= q.Inverse().Rotate({m, 0, 0});
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            tr -= q.Inverse().Rotate({0, m, 0});
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            tr += q.Inverse().Rotate({0, m, 0});
        }

        dp(tr);

        prog.SetFloat("uTime", glfwGetTime());
        prog.SetQuaternion("q", q);
        prog.SetVec3("tr", tr);
        prog.SetFloat("sc", sc);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        prog.SetInt("texture1", 0);
        prog.SetInt("texture2", 1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            prog.Use();
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);
        }

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);


    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER)) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}
