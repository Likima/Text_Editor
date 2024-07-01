#include <iostream>

#include <SDL2/SDL.h>
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL_opengl.h>

#define GL_GLEXT_PROTOTYPES
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H  

#include "input_processing.hpp"
#include "initialization.hpp"
#include "shader_processing.hpp"

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

std::map<GLchar, Character> Characters;

const char* VERTEX_SHADER_PATH = "../shaders/font-vertex.glsl";
const char* FRAG_SHADER_PATH = "../shaders/font-frag.glsl";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

bool loadFont(const char* fontPath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}

int main() {

    loadFont("../Fonts/VictorMono-Regular.ttf");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Creation of window

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    

    GLFWwindow* window = Create_Window();
    if(!window) {
        std::cout << "Unknown Failure"<<std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!initGLEW) {
        std::cout << "Error Initializing GLEW" << std::endl;
    }
    glfwSetKeyCallback(window, key_callback);


    glewExperimental = GL_TRUE;
    createShaderProgram(VERTEX_SHADER_PATH, FRAG_SHADER_PATH);
    //bool quit = false;
    //while(!quit) {
        while (!glfwWindowShouldClose(window)) {
            // Render here
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }
    //}

    glfwDestroyWindow(window);
    glfwTerminate();
    //bool quit = false;
    //while(!quit){
    //    SDL_Event event = {0};
    //    while(SDL_PollEvent(&event)){
    //        switch(event.type){
    //            case(SDL_QUIT): {
    //                quit = true;
    //            }
    //            break;
    //            case(SDL_TEXTINPUT): { // Retrieve text from user.
    //                std::cout<<event.text.text<<std::endl;
    //            }
    //            break;
    //        }
    //    }
    //}
    return 0;
}
