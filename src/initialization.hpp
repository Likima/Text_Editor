#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

bool initGLEW();
void error_callback(int error, const char* description);
bool loadFont();

#endif // INITIALIZATION_HPP