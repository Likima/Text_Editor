#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> // For glm::ortho
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr
;
void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale);
void renderCursor(GLuint &s, float x, float y, float scale, glm::vec3 color);
bool read_from_file(std::string &file_name);
void renderLineNumbers(GLuint &s, float x, float y, glm::vec3 color);

#endif