#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::ortho
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr

#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

#include "globals.hpp"

// Defined (external) in globals.hpp
std::map<GLchar, Character> Characters;
int SCREEN_HEIGHT = 600;
int SCREEN_WIDTH = 800;
GLuint program = 0;
unsigned int VAO, VBO;
std::string save_file;
bool scrolling = true;
Editor e;
// ---

std::string operator*(const std::string &str, int times)
{
    std::string result;
    result.reserve(str.size() * times); // Reserve space to avoid multiple reallocations
    for (int i = 0; i < times; ++i)
    {
        result += str;
    }
    return result;
}

// Overload the * operator to handle the case where the integer comes first
std::string operator*(int times, const std::string &str)
{
    return str * times;
}

#include "lexer.hpp"
#include "initialization.hpp"
#include "shader_processing.hpp"
#include "editor.hpp"

// Global Definitions
const char *VERTEX_SHADER_PATH = "../shaders/font-vertex.glsl";
const char *FRAG_SHADER_PATH = "../shaders/font-frag.glsl";
float x_Padding = 15.0f;
float y_Padding = FONT_SIZE + 15.0f;
//----

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if (argc >= 3)
        {
            std::string toggle = argv[2];
            if (toggle == "1")
                scrolling = false;
        }
        save_file = argv[1];
        if (!read_from_file(save_file))
        {
            {
                std::ofstream outputFile(save_file);
                if (!outputFile.is_open())
                {
                    std::cerr << "Failed to create file: " << save_file << std::endl;
                    return -1;
                }

                for (const std::string &line : e.lines)
                {
                    outputFile << line << std::endl;
                }

                outputFile.close();
                e.tab_offset_vec.push_back(0);
            }
        }
    }
    else
        e.tab_offset_vec.push_back(0);
    glfwSetErrorCallback(error_callback);
    // Creation of window
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DECORATED, false); // <--- For Removing window border. Deal with in the future?
    glfwWindowHint(GLFW_FLOATING, true);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Text Editor", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    int gl_ver_major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int gl_ver_minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    printf("OpenGL %d.%d\n", gl_ver_major, gl_ver_minor);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    load_gl_extensions();

    if (!initGLEW)
    {
        std::cout << "Error Initializing GLEW" << std::endl;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint vert_shader = 0;
    GLuint frag_shader = 0;

    if (!compileShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, &vert_shader))
    {
        std::cout << "Failure to compile vertex shader" << std::endl;
        return (-1);
    }
    if (!compileShader(FRAG_SHADER_PATH, GL_FRAGMENT_SHADER, &frag_shader))
    {
        std::cout << "Failure to compile frag shader " << std::endl;
        return (-1);
    }
    if (!createShaderProgram(vert_shader, frag_shader, &program))
    {
        std::cout << "Failure to link programs" << std::endl;
        return (-1);
    }

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
    glUseProgram(program);
    GLuint mLocation = glGetUniformLocation(program, "projection");
    if (mLocation == -1)
    {
        std::cerr << "Failed to find uniform location for 'projection'" << std::endl;
        return -1;
    }
    glUniformMatrix4fv(mLocation, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    loadFont();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(1);

    int prevNumLines = 0;

    while (!glfwWindowShouldClose(window))
    {
        float timeValue = glfwGetTime();

        // Use the shader program
        glUseProgram(program);

        // Set the time uniform
        GLuint timeLocation = glGetUniformLocation(program, "time");
        glUniform1f(timeLocation, timeValue);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderCursor(program, x_Padding, float(SCREEN_HEIGHT) - y_Padding, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        // if (e.lines.size() != prevNumLines || prevNumLines == 0)
        //{
        //     renderLineNumbers(program, x_Padding - FONT_SIZE * 3, float(SCREEN_HEIGHT) - y_Padding, glm::vec3(0.0f, 1.0f, 0.0f));
        // }
        // prevNumLines = e.lines.size();
        renderLineNumbers(program, x_Padding - FONT_SIZE * 3, float(SCREEN_HEIGHT) - y_Padding, glm::vec3(0.0f, 1.0f, 0.0f));
        renderText(program, e.lines, x_Padding, float(SCREEN_HEIGHT) - y_Padding, 1.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
