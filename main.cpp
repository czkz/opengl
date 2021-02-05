#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"
#include <Vector.h>
#include <Quaternion.h>
#include <dbg.h>
#include "Camera.h"
#include "FrameCounter.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow*, Camera&, float);
void mouse_callback(GLFWwindow* window, double x, double y);

// settings
const unsigned int windowWidth = 1000;
const unsigned int windowHeight = 1000;

Camera camera = { {0, 0, 0}, Quaternion::Identity() };
Vector3 cameraEuler = {0, 0, 0};


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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    float sc = 1;
    FrameCounter frameCounter;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        frameCounter.tick();
        glfwPollEvents();
        processInput(window, camera, frameCounter.deltaTime / 16);

        camera.rotation = Quaternion::Euler(cameraEuler);

        prog.SetFloat("uTime", glfwGetTime());
        prog.SetQuaternion("q", camera.rotation);
        prog.SetVec3("tr", camera.position);
        prog.SetFloat("sc", sc);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        prog.SetInt("texture1", 0);
        prog.SetInt("texture2", 1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClearColor(1., 1., 1., 1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            // First two commands can be outside the loop
            //  only when there's one shader and one VAO
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

void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ENTER)) {
        glfwSetWindowShouldClose(window, true);
    }

    constexpr float r = 1 * M_PI/180.f;
    constexpr float m = 0.05;
    // if (glfwGetKey(window, GLFW_KEY_UP)) {
    //     camera.RotateX(-r * deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_DOWN)) {
    //     camera.RotateX(+r * deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_LEFT)) {
    //     camera.RotateY(-r * deltaTime);
    // }
    // if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
    //     camera.RotateY(+r * deltaTime);
    // }
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        camera.RotateZ(-r * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        camera.RotateZ(+r * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        camera.Translate(Vector3 {0, 0, -m} * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        camera.Translate(Vector3 {0, 0, +m} * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        camera.Translate(Vector3 {-m, 0, 0} * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        camera.Translate(Vector3 {+m, 0, 0} * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        camera.Translate(Vector3 {0, -m, 0} * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        camera.Translate(Vector3 {0, +m, 0} * deltaTime);
    }

}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow*, double x, double y) {
    static Vector2 prev (x, y);
    const Vector2 curr (x, y);
    Vector2 v = curr - prev;
    prev = curr;

    constexpr float sensitivity = 0.002;
    v *= sensitivity;

    cameraEuler += {v.y, v.x, 0};
    cameraEuler.x = std::clamp<VEC_REAL_T>(cameraEuler.x, -M_PI/2, +M_PI/2);
    // camera.RotateX(v.y);
    // camera.RotateY(v.x);
}
