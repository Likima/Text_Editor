#include "shader_processing.hpp"

char *slurp_file_into_malloced_cstr(const char *file_path) {
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

bool compileShaderFromSource(const GLchar *source, GLenum type, GLuint *shader) {
    *shader = glCreateShader(type);
    std::cout << "Made it past here!" << std::endl;
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint success = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool compileShader(const char* filePath, GLenum type, GLuint* shader) {
    char* slurped_file = slurp_file_into_malloced_cstr(filePath);
    if (!slurped_file) {
        std::cerr << "Failed to read file: " << filePath << std::endl;
        return false;
    }
    std::cout << "Shader file contents:\n" << slurped_file << std::endl;

    bool compiled = compileShaderFromSource(slurped_file, type, shader);
    if (!compiled) {
        std::cerr << "Failed to compile shader" << std::endl;
    }

    free(slurped_file);
    return compiled;
}
