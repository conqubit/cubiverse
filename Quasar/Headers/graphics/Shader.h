#pragma once

#include "math/Vector.h"

class Model;

class Shader {
public:
	GLuint program;

	Shader() :
	program(0) {
	}

	bool Init(const string& vertexShaderFile, const string& fragmentShaderFile);
	void Shutdown();
	void Bind();
	void Unbind();

	static GLuint MakeShader(GLenum type, const string& filename);
	static GLuint MakeProgram(GLuint vertexShader, GLuint fragmentShader);
	static void ShowInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
};