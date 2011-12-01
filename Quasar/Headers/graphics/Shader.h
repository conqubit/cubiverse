#pragma once

#include "math/Vector.h"

class Model;

class Shader {
public:
    GLuint program;

    Shader();
    ~Shader();
    bool Init(string vertexShaderFile, string fragmentShaderFile);
    void Shutdown();
    void Bind();
    void Unbind();

    static GLuint MakeShader(GLenum type, string filename);
    static GLuint MakeProgram(GLuint vertexShader, GLuint fragmentShader);
    static void ShowInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
};