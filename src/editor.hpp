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
bool read_from_file(std::string &file_name);
void enter_to_editor();
void editor_up();
void editor_down();
void editor_left();
void editor_right();
void editor_backspace();

#endif