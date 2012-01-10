#pragma once

#include <boost/thread.hpp>

// C header files.
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>

// C++ header files.
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <vector>
#include <stack>
#include <set>
#include <map>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <lodepng.h>

#include <windows.h>

#ifdef _WIN32
#define SYSTEM_WINDOWS
#endif

typedef uint8_t			byte;

typedef unsigned short	ushort;
typedef unsigned int	uint;

typedef std::string string;

// General purpose string conversion.
template <typename T>
string str(T x, int prec = -1) {
	std::stringstream ss;
	if (prec != -1) ss.precision(prec);
	ss << std::fixed << x;
	return ss.str();
}

template <typename T>
void print(const T& x) {
	std::cout << x << std::endl;
}

template <typename T>
void printerr(const T& x) {
	std::cerr << x << std::endl;
}

#define ZeroStruct(s) memset(&(s), 0, sizeof(s))

#define MEASURE(tag, code) {double t=glfwGetTime();{code}t=glfwGetTime()-t;printf("%s: %f ms\n",tag,t/1000);}

// Common.
#include "math/Math.h"
#include "Res.h"
#include "Config.h"
#include "util/String.h"