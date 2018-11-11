#pragma once
#include <gl/freeglut.h>

struct Weather {
	float x;
	float y;
	float z;
};

Weather snow[100];
Weather rain[100];

static int selectWeather = 0;