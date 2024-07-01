#include "glextloader.hpp"


static PFNGLCREATESHADERPROC glCreateShader = NULL;
static PFNGLSHADERSOURCEPROC glShaderSource = NULL;
static PFNGLCOMPILESHADERPROC glCompileShader = NULL;
static PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
static PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
static PFNGLATTACHSHADERPROC glAttachShader = NULL;
static PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
static PFNGLDELETESHADERPROC glDeleteShader = NULL;
static PFNGLUSEPROGRAMPROC glUseProgram = NULL;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = NULL;
static PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
static PFNGLUNIFORM2FPROC glUniform2f = NULL;
static PFNGLGENBUFFERSPROC glGenBuffers = NULL;
static PFNGLBINDBUFFERPROC glBindBuffer = NULL;
static PFNGLBUFFERDATAPROC glBufferData = NULL;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
static PFNGLUNIFORM1FPROC glUniform1f = NULL;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = NULL;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
static PFNGLUNIFORM1IPROC glUniform1i = NULL;
static PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;
static PFNGLUNIFORM4FPROC glUniform4f = NULL;
// TODO: there is something fishy with Windows gl.h header
// Let's try to ship our own gl.h just like glext.h
#ifdef _WIN32
static PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
#endif // _WIN32

static void load_gl_extensions(void)
{
    // TODO: check for failtures?
    // Maybe some of the functions are not available
    glCreateShader            = (PFNGLCREATESHADERPROC) glfwGetProcAddress("glCreateShader");
    glShaderSource            = (PFNGLSHADERSOURCEPROC) glfwGetProcAddress("glShaderSource");
    glCompileShader           = (PFNGLCOMPILESHADERPROC) glfwGetProcAddress("glCompileShader");
    glGetShaderiv             = (PFNGLGETSHADERIVPROC) glfwGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC) glfwGetProcAddress("glGetShaderInfoLog");
    glAttachShader            = (PFNGLATTACHSHADERPROC) glfwGetProcAddress("glAttachShader");
    glCreateProgram           = (PFNGLCREATEPROGRAMPROC) glfwGetProcAddress("glCreateProgram");
    glLinkProgram             = (PFNGLLINKPROGRAMPROC) glfwGetProcAddress("glLinkProgram");
    glGetProgramiv            = (PFNGLGETPROGRAMIVPROC) glfwGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog       = (PFNGLGETPROGRAMINFOLOGPROC) glfwGetProcAddress("glGetProgramInfoLog");
    glDeleteShader            = (PFNGLDELETESHADERPROC) glfwGetProcAddress("glDeleteShader");
    glUseProgram              = (PFNGLUSEPROGRAMPROC) glfwGetProcAddress("glUseProgram");
    glGenVertexArrays         = (PFNGLGENVERTEXARRAYSPROC) glfwGetProcAddress("glGenVertexArrays");
    glBindVertexArray         = (PFNGLBINDVERTEXARRAYPROC) glfwGetProcAddress("glBindVertexArray");
    glDeleteProgram           = (PFNGLDELETEPROGRAMPROC) glfwGetProcAddress("glDeleteProgram");
    glGetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC) glfwGetProcAddress("glGetUniformLocation");
    glUniform2f               = (PFNGLUNIFORM2FPROC) glfwGetProcAddress("glUniform2f");
    glGenBuffers              = (PFNGLGENBUFFERSPROC) glfwGetProcAddress("glGenBuffers");
    glBindBuffer              = (PFNGLBINDBUFFERPROC) glfwGetProcAddress("glBindBuffer");
    glBufferData              = (PFNGLBUFFERDATAPROC) glfwGetProcAddress("glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) glfwGetProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer     = (PFNGLVERTEXATTRIBPOINTERPROC) glfwGetProcAddress("glVertexAttribPointer");
    glUniform1f               = (PFNGLUNIFORM1FPROC) glfwGetProcAddress("glUniform1f");
    glBufferSubData           = (PFNGLBUFFERSUBDATAPROC) glfwGetProcAddress("glBufferSubData");
    glGenFramebuffers         = (PFNGLGENFRAMEBUFFERSPROC) glfwGetProcAddress("glGenFramebuffers");
    glBindFramebuffer         = (PFNGLBINDFRAMEBUFFERPROC) glfwGetProcAddress("glBindFramebuffer");
    glFramebufferTexture2D    = (PFNGLFRAMEBUFFERTEXTURE2DPROC) glfwGetProcAddress("glFramebufferTexture2D");
    glCheckFramebufferStatus  = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) glfwGetProcAddress("glCheckFramebufferStatus");
    glUniform1i               = (PFNGLUNIFORM1IPROC) glfwGetProcAddress("glUniform1i");
    glDrawBuffers             = (PFNGLDRAWBUFFERSPROC) glfwGetProcAddress("glDrawBuffers");
    glUniform4f               = (PFNGLUNIFORM4FPROC) glfwGetProcAddress("glUniform4f");
#ifdef _WIN32
    glActiveTexture           = (PFNGLACTIVETEXTUREPROC) glfwGetProcAddress("glActiveTexture");
#endif // _WIN32

    if (glfwExtensionSupported("GL_ARB_debug_output")) {
        fprintf(stderr, "INFO: ARB_debug_output is supported\n");
        glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) glfwGetProcAddress("glDebugMessageCallback");
    } else {
        fprintf(stderr, "WARN: ARB_debug_output is NOT supported\n");
    }

    if (glfwExtensionSupported("GL_EXT_draw_instanced")) {
        fprintf(stderr, "INFO: EXT_draw_instanced is supported\n");
        glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) glfwGetProcAddress("glDrawArraysInstanced");
    } else {
        fprintf(stderr, "WARN: EXT_draw_instanced is NOT supported\n");
    }
}