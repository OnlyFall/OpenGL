#pragma once
#include <gl/freeglut.h>

struct Robot {
	float x;
	float y = 50;
	float z;

	float rad;

	BOOL walk;
};

static Robot robot;


GLvoid RobotMiddle()
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0, -15, 0);
	glScalef(1, 1.2, 0.5);
	glutSolidCube(20);
	glPopMatrix();
}

GLvoid RobotLeg()
{
	glPushMatrix();

	glPopMatrix();

	glPushMatrix();

	glPopMatrix();
}

GLvoid RobotHand()
{

}

GLvoid RobotHead()
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(10, 10, 10);
	glPopMatrix();
}