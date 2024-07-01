#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include <iostream>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL_opengl.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
;
GLFWwindow* Create_Window();
bool initGLEW();

#endif // INITIALIZATION_HPP