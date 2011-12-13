#pragma once

#include <SDKDDKVer.h>
#include <windows.h>

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
#include <set>
#include <map>

#include <GL/glew.h>

// SFML header files.
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Integer typedefs for convenience and readability.
typedef unsigned char          byte;
typedef unsigned short         ushort;
typedef unsigned int           uint;

typedef std::int16_t           int16;
typedef std::uint16_t          uint16;
typedef std::int32_t           int32;
typedef std::uint32_t          uint32;
typedef std::int64_t           int64;
typedef std::uint64_t          uint64;

// Miscellaneous typedefs for convenience and readability.
typedef std::string string;
typedef sf::Keyboard::Key Key;
typedef sf::Event::EventType EventType;

// General purpose string conversion.
template <typename T>
string str(T x, int prec = -1) {
    std::stringstream ss;
    if (prec != -1) ss.precision(prec);
    ss << std::fixed << x;
    return ss.str();
}

// General purpose stdout printing function.
template <typename T>
void print(T x) {
    std::cout << x << std::endl;
}

// General purpose stderr printing function.
template <typename T>
void printerr(T x) {
    std::cerr << x << std::endl;
}

// Custom math header file.
#include "math/Math.h"

#define ZeroStruct(s) memset(&(s), 0, sizeof(s))

// Rudimentary macro to print out how much time a block of code takes to run.
#define MEASURE(tag, code) {clock_t t=clock();{code}t=clock()-t;printf("%s: %d ms\n",tag,t);}

// Rudimentary macro to print out how much time a block of code takes to run.
#define ACCURATE_MEASURE(tag, code) {int64 _s, _e, _f; \
                                     QueryPerformanceFrequency((LARGE_INTEGER*)&_f); \
                                     QueryPerformanceCounter((LARGE_INTEGER*)&_s); \
                                     {code} \
                                     QueryPerformanceCounter((LARGE_INTEGER*)&_e); \
                                     printf("%s: %f ms\n", tag, (double)(_e-_s)/_f*1000);}