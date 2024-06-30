#include <iostream>

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include "glfw_functions.hpp"
#include "render.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window =
        SDL_CreateWindow("Hello World!",
                         0, 0,
                         SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    // Creation of a window

    if (window == NULL) {
        fprintf(stderr, "ERROR: Could not create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        int major;
        int minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
        printf("GL version %d.%d\n", major, minor);
    }

    if (SDL_GL_CreateContext(window) == NULL) {
        fprintf(stderr, "ERROR: Could not create OpenGL context: %s\n", SDL_GetError());
        return 1;
    }

    bool quit = false;
    while(!quit){
        SDL_Event event = {0};
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case(SDL_QUIT): {
                    quit = true;
                }
                break;
            }
        }
    }
    return 0;
}
