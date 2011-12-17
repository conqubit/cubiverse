#include "stdafx.h"

#include "system.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"

Shader::Shader() :
program() {
}

Shader::~Shader() {
}

bool Shader::Init(const string& vertexShaderFile, const string& fragmentShaderFile) {
    GLuint vertexShader = MakeShader(GL_VERTEX_SHADER, vertexShaderFile);
    if (!vertexShader) return false;

    GLuint fragmentShader = MakeShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
    if (!fragmentShader) return false;

    program = MakeProgram(vertexShader, fragmentShader);
    if (!program) return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::Shutdown() {
    glDeleteProgram(program);
}

void Shader::Bind() {
    glUseProgram(program);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::ShowInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGetiv, PFNGLGETSHADERINFOLOGPROC glGetInfoLog) {
    GLint length = 0;
    glGetiv(object, GL_INFO_LOG_LENGTH, &length);
    char* log = (char*)malloc(length);
    glGetInfoLog(object, length, 0, log);
    printf(log);
    free(log);
}

GLuint Shader::MakeShader(GLenum type, const string& filename) {
    GLuint shader = 0;

    FILE* f = fopen(filename.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    char* contents = (char*)malloc(length);
    if (!contents) {
        fclose(f);
        return 0;
    }
    rewind(f);
    fread(contents, 1, length, f);
    fclose(f);

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&contents, (GLint*)&length);
    free(contents);
    glCompileShader(shader);

    GLint shaderOk = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderOk);
    if (!shaderOk) {
        printf("Failed to compile %s:\n", filename.c_str());
        ShowInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Shader::MakeProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint programOk = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &programOk);
    if (!programOk) {
        printf("Failed to link shader program:\n");
        ShowInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}