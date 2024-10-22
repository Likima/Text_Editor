#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <regex>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::ortho
#include <glm/gtc/type_ptr.hpp>         // For glm::value_ptr
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Editor
{
    std::vector<std::string> lines = {""};
    std::vector<int> tab_offset_vec = {};
    int num_lines;
    int characters_per_line;
    int cursor_x = 0; // TO DO : Update cursor_x
    int cursor_y = 0;
    int greatest_cursor_x = 0;
};

extern Editor e;

#define FONT_SIZE 20
#define FONT_PATH "../Fonts/VictorMono-Regular.ttf"
#define SIMPLE_VERTICIES_CAP 1920000
;
struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

extern std::map<GLchar, Character> Characters;

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

extern std::string save_file;

extern bool scrolling;

extern GLuint program;

extern unsigned int VAO, VBO;

extern bool CTRL_PRESSED;

extern bool text_changed;
extern bool line_nums_changed;

extern std::string operator*(const std::string &str, int times);

// Overload the * operator to handle the case where the integer comes first
extern std::string operator*(int times, const std::string &str);

extern float x_Padding;
extern float y_Padding;

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    DELIMITER,
    COMMENT,
    UNKNOWN
};

struct Token
{
    TokenType type;
    std::string text;
    bool chroma;
    glm::vec3 color;
};

extern std::vector<std::pair<TokenType, std::regex>> tokenDefinitions;
extern std::unordered_map<TokenType, glm::vec3> tokenColors;

extern std::vector<char> delimiters;
extern std::string TAB;

std::vector<Token> tokenize(const std::string &code);
void renderText(GLuint &s, std::vector<std::string> text, float x, float y, float scale);
void push_to_editor(std::string s);

// Not too sure what this does. Fixes seg fault though!
static void load_gl_extensions(void)
{
    // TODO: check for failtures?
    // Maybe some of the functions are not available
    glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
    glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    glUniform2f = (PFNGLUNIFORM2FPROC)glfwGetProcAddress("glUniform2f");
    glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    glUniform1f = (PFNGLUNIFORM1FPROC)glfwGetProcAddress("glUniform1f");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glfwGetProcAddress("glBufferSubData");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glfwGetProcAddress("glGenFramebuffers");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glfwGetProcAddress("glBindFramebuffer");
    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glfwGetProcAddress("glFramebufferTexture2D");
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glfwGetProcAddress("glCheckFramebufferStatus");
    glUniform1i = (PFNGLUNIFORM1IPROC)glfwGetProcAddress("glUniform1i");
    glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glfwGetProcAddress("glDrawBuffers");
    glUniform4f = (PFNGLUNIFORM4FPROC)glfwGetProcAddress("glUniform4f");
    glUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glfwGetProcAddress("glBufferSubData");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)glfwGetProcAddress("glUniform3fv");
    glDrawArraysEXT = (PFNGLDRAWARRAYSEXTPROC)glfwGetProcAddress("glDrawArraysEXT");
#ifdef _WIN32
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");
#endif // _WIN32

    if (glfwExtensionSupported("GL_ARB_debug_output"))
    {
        fprintf(stderr, "INFO: ARB_debug_output is supported\n");
        glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)glfwGetProcAddress("glDebugMessageCallback");
    }
    else
    {
        fprintf(stderr, "WARN: ARB_debug_output is NOT supported\n");
    }

    if (glfwExtensionSupported("GL_EXT_draw_instanced"))
    {
        fprintf(stderr, "INFO: EXT_draw_instanced is supported\n");
        glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glfwGetProcAddress("glDrawArraysInstanced");
    }
    else
    {
        fprintf(stderr, "WARN: EXT_draw_instanced is NOT supported\n");
    }
}

#endif