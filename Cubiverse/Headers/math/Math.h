#pragma once

#include <math.h>

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

#define SQR(x)	((x) * (x))
#define ABS(x)	((x) < 0 ? (-(x)) : (x))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#define SIGN(x)   ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define RAND32() (rand()^(rand()<<15)^(rand()<<30))

#define PI	 3.14159265358979323846
#define PI_2   1.57079632679489661923
#define SQRT_2 1.41421356237309504880
#define PHI	1.61803398874989484820

#include "math/Vector.h"
#include "math/BoundingBox.h"
#include "math/Color.h"