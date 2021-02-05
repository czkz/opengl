#pragma once
#include <glad/glad.h>

class Model {
public:
    GLuint vao;
    GLuint vbo;
    GLuint ubo;

    Model() {
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

    ~Model() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};
