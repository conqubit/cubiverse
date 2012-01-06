#pragma once

// C header files.
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>

// C++ header files.
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdint>

#include <vector>
#include <stack>
#include <set>
#include <map>

#include <GL/glew.h>

// SFML header files.
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifdef SFML_SYSTEM_WINDOWS
#include <windows.h>
#endif

typedef unsigned char	byte;
typedef unsigned short	ushort;
typedef unsigned int	uint;

typedef std::int64_t	int64;
typedef std::uint64_t	uint64;

typedef std::string string;
typedef sf::Keyboard::Key Key;

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

#define MEASURE(tag, code) {clock_t t=clock();{code}t=clock()-t;printf("%s: %d ms\n",tag,t);}

#define ACCURATE_MEASURE(tag, code) {int64 _s, _e, _f; \
									 QueryPerformanceFrequency((LARGE_INTEGER*)&_f); \
									 QueryPerformanceCounter((LARGE_INTEGER*)&_s); \
									 {code} \
									 QueryPerformanceCounter((LARGE_INTEGER*)&_e); \
									 printf("%s: %f ms\n", tag, (double)(_e-_s)/_f*1000);}

// Common.
#include "math/Math.h"
#include "Res.h"
#include "Config.h"
#include "util/String.h"