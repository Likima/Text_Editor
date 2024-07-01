#ifndef SHADER_PROCESSING_HPP
#define SHADER_PROCESSING_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

bool compileShader(const char* source, GLenum type, GLuint* shader);
bool compileShaderFromSource(const GLchar *source, GLenum type, GLuint *shader);
bool createShaderProgram(GLuint vert_shader, GLuint frag_shader, GLuint *program);

#endif //SHADER_PROCESSING.HPP