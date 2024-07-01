#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>



char *slurp_file_into_malloced_cstr(const char *file_path)
{
    // Credit to tsoding for this function
    FILE *f = NULL;
    char *buffer = NULL;
    long size = 0;

    f = fopen(file_path, "r");
    if (f == NULL) {
        goto fail;
    }
    if (fseek(f, 0, SEEK_END) < 0) {
        goto fail;
    }

    size = ftell(f);
    if (size < 0) {
        goto fail;
    }

    buffer = (char*)malloc(size + 1);
    if (buffer == NULL) {
        goto fail;
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        goto fail;
    }

    fread(buffer, 1, size, f);
    if (ferror(f)) {
        goto fail;
    }

    buffer[size] = '\0';

    if (f) {
        fclose(f);
        errno = 0;
    }
    return buffer;

fail:
    if (f) {
        int saved_errno = errno;
        fclose(f);
        errno = saved_errno;
    }
    if (buffer) {
        free(buffer);
    }
    return NULL;
}

GLuint* compileShader(const char* source, GLenum type) {
    const char* slurped_file = slurp_file_into_malloced_cstr(source);
    GLuint *shader;
    std::cout<< slurp_file_into_malloced_cstr(source) << std::endl;
    
    *shader = glCreateShader(type);

    glShaderSource(*shader, 1, &slurped_file, NULL);
    glCompileShader(*shader);

    GLint success;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint* createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint* vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint* fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint* shaderProgram;
    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, *vertexShader);
    glAttachShader(*shaderProgram, *fragmentShader);
    glLinkProgram(*shaderProgram);

    GLint success;
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(*shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(*vertexShader);
    glDeleteShader(*fragmentShader);

    return shaderProgram;
}
