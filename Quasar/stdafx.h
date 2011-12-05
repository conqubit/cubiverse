#pragma once

#include <SDKDDKVer.h>

// C header files.
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>
#include <stdint.h>

// C++ header files.
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

// SFML header files.
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Integer typedefs for convenience and readability.
typedef uint8_t  byte;
typedef int8_t   sbyte;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef int64_t  int64;
typedef uint64_t uint64;

// Miscellaneous typedefs for convenience and readability.
typedef std::string string;
typedef sf::Keyboard::Key Key;
typedef sf::Event::EventType EventType;

// General purpose string conversion.
template <typename T>
string str(T x) {
    std::stringstream ss;
    ss.precision(2);
    ss << std::fixed << x;
    return ss.str();
}

// General purpose stdout printing function.
template <typename T>
void print(T x) {
    std::cout << x << '\n';
}

// General purpose stderr printing function.
template <typename T>
void printerr(T x) {
    std::cerr << x << '\n';
}

// Custom math header file.
#include "math/Math.h"

#define ZeroStruct(s) memset(&(s), 0, sizeof(s))

// Rudimentary macro to print out how much time a block of code takes to run.
#define MEASURE(tag, code) {clock_t t=clock();{code}t=clock()-t;printf("%s: %d ms\n",tag,t);}