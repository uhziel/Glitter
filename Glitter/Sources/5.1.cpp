// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

const char* vertex_shader_source = "#version 330 core\n\
\n\
layout (location = 0) in vec3 position;\n\
\n\
void main()\n\
{\n\
    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
}\n\
";

const char* fragment_shader_source = "#version 330 core\n\
out vec4 color;\n\
void main()\n\
{\n\
    color = vec4(1.0f,0.0f, 0.0f, 1.0f);\n\
}\n\
";

void ExecShaderProgram() {
    // Shader Program
    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    GLint success;
    GLchar info_log[512] = {0};
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        fprintf(stderr, "compile SHADER success.\n");
    }
    else
    {
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "compile SHADER fail, reason:%s", info_log);
    }
    // program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(shader_program);
}

int main() {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // Ready
    ExecShaderProgram();

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Work
        // Put vertices to GPU memory
        GLfloat vertices[6][3] = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f},
            {0.5f, 0.5f, 0.0f}
        };

        GLuint indices[6] = {
            0, 1, 2,
            2, 3, 0
        };
        GLuint VBO = 0;
        glGenBuffers(1, &VBO);

        GLuint EBO = 0;
        glGenBuffers(1, &EBO);

        GLuint VAO = 0;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), nullptr);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
