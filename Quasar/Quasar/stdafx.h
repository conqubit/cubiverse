// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <iostream>

typedef std::string string;

template <typename T>
string str(T x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

template <typename T>
void print(T x) {
    std::cout << x;
}

// TODO: reference additional headers your program requires here
#include "math/math.h"

#define rand30() (rand() ^ (rand() << 15))

#define MEASURE(tag, code) {		\
    clock_t t = clock();			\
    code							\
    t = clock()-t;					\
    printf("%s: %d ms\n", tag, t);	\
}