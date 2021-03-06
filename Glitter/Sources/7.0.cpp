// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"

// System Headers #include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>


const char* vertex_shader_source = "#version 330 core\n\
\n\
layout (location = 0) in vec3 position;\n\
layout (location = 1) in vec3 color;\n\
layout (location = 2) in vec2 _tex_coor;\n\
out vec4 vertex_color;\n\
out vec2 tex_coor;\n\
\n\
void main()\n\
{\n\
    gl_Position = vec4(position.x, -position.y, position.z, 1.0);\n\
    vertex_color = vec4(color, 1.0f);\n\
    tex_coor = _tex_coor;\n\
}\n\
";

const char* fragment_shader_source = "#version 330 core\n\
in vec4 vertex_color;\n\
in vec2 tex_coor;\n\
out vec4 color;\n\
uniform sampler2D our_texture;\n\
void main()\n\
{\n\
    color = texture(our_texture, -tex_coor) * vertex_color;\n\
}\n\
";

const char* fragment_shader_source2 = "#version 330 core\n\
out vec4 color;\n\
uniform vec4 our_color;\n\
void main()\n\
{\n\
    color = our_color;\n\
}\n\
";

GLint CreateShaderProgram(const char** vs, const char** fs) {
    // Shader Program
    GLint success;
    GLchar info_log[512] = {0};
    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vs, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        fprintf(stderr, "compile vs success.\n");
    }
    else
    {
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "compile vs fail, reason:%s", info_log);
    }
    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fs, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        fprintf(stderr, "compile fs success.\n");
    }
    else
    {
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "compile fs fail, reason:%s", info_log);
    }
    // program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint CreateVAO(const GLfloat* vertices, GLuint size_v,
                const GLuint* indices, GLuint size_i) {
        GLuint VBO = 0;
        glGenBuffers(1, &VBO);

        GLuint EBO = 0;
        glGenBuffers(1, &EBO);

        GLuint VAO = 0;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size_v, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(3*sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(6*sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);

        return VAO;
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
    GLuint prog1 = CreateShaderProgram(&vertex_shader_source, &fragment_shader_source);
    glUseProgram(prog1);
    // Image
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *data = stbi_load("Glitter/IMG_0415.jpg", &width, &height, & channels, 0);
    fprintf(stderr, "image width:%d height:%d\n", width, height);
    // Texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Work1
        // Put vertices to GPU memory
        GLfloat vertices[4][8] = {
            {0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
            {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},
            {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}
        };

        GLuint indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        GLuint VAO = CreateVAO(vertices[0], sizeof(vertices), indices, sizeof(indices));

        glBindVertexArray(VAO);
        glUseProgram(prog1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
