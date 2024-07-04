#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> // For glm::ortho
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr
;

void push_to_editor(std::string s);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow *window, unsigned int codepoint);
void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale, glm::vec3 color);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif