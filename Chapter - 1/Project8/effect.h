#pragma once
#include <gl/freeglut.h>

struct Effect {
	float x;
	float y;
	float z;

	int dir;
	float count = 0;
};