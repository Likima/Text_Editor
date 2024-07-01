#ifndef SHADER_PROCESSING_HPP
#define SHADER_PROCESSING_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint compileShader(const char* source, GLenum type);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

#endif //SHADER_PROCESSING.HPP