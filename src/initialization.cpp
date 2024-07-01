#include "initialization.hpp"

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

bool initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
    return true;
}
