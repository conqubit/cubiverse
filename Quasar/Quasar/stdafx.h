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
    std::cout << x << '\n';
}

#define ZeroStruct(s) memset(&(s), 0, sizeof(s))

// TODO: reference additional headers your program requires here
#include "math/math.h"

#define rand30() (rand() ^ (rand() << 15))
#define rand32() (rand() ^ (rand() << 15) ^ (rand() << 30))

#define MEASURE(tag, code) {clock_t t=clock();{code}t=clock()-t;printf("%s: %d ms\n",tag,t);}

#define VEC3_RANGE(vec) Vector3I p, _v = vec; \
                        for(p.x = 0; p.x < _v.x; p.x++) \
                        for(p.y = 0; p.y < _v.y; p.y++) \
                        for(p.z = 0; p.z < _v.z; p.z++)

#define VEC3_RANGE_AB(start, end) Vector3I p, _s = start, _e = end; \
                                  for(p.x = _s.x; p.x < _e.x; p.x++) \
                                  for(p.y = _s.y; p.y < _e.y; p.y++) \
                                  for(p.z = _s.z; p.z < _e.z; p.z++)

#define VEC3_RANGE_AB_INC(start, end) Vector3I p, _s = start, _e = end; \
                                      for(p.x = _s.x; p.x <= _e.x; p.x++) \
                                      for(p.y = _s.y; p.y <= _e.y; p.y++) \
                                      for(p.z = _s.z; p.z <= _e.z; p.z++)

#define VEC3_RANGE_OFFSET(off, vec) Vector3I p, _o = off, _v = _o + vec; \
                                    for(p.x = _o.x; p.x < _v.x; p.x++) \
                                    for(p.y = _o.y; p.y < _v.y; p.y++) \
                                    for(p.z = _o.z; p.z < _v.z; p.z++)