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


GLvoid wearherSnow()
{
	for (int i = 0; i < 100; ++i) {
		glPushMatrix();
		glTranslatef(snow[i].x, snow[i].y, snow[i].z);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(4, 4, 4);
		glPopMatrix();
	}
}