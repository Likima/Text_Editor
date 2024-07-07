#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "globals.hpp"

void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale);
void renderCursor(GLuint &s, float x, float y, float scale, glm::vec3 color);
void renderLineNumbers(GLuint &s, float x, float y, glm::vec3 color);
void updateProjection(GLuint program, int xpos, int ypos);

#endif