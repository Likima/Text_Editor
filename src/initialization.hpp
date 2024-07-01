#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
;
bool initGLEW();
void error_callback(int error, const char* description);

#endif // INITIALIZATION_HPP