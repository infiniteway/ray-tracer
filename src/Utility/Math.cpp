#include "Math.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

float Math::InterpolationQuad4f(const float dy, const float dz,
								const float x1, const float x2,
								const float x3, const float x4) {
	assert(dy >= 0 && dy <= 1 + FLT_EPSILON);
	assert(dz >= 0 && dz <= 1 + FLT_EPSILON);
	const float idy = 1 - dy;
	const float idz = 1 - dz;
	float a1 = idy * dz; // lower right area.
	float a2 = dy * dz; // lower left area.
	float a3 = dy * idz; // upper left area.
	float a4 = idy * idz; // upper right area.
	return a3 * x1 + a4 * x2 + a1 * x3 + a2 * x4;
}
