#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

void save_to_file(int added = 0);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void char_callback(GLFWwindow *window, unsigned int codepoint);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif