#pragma once
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

GLvoid RobotLeg(int rad)
{
	glColor3f(1.0f, 0.4f, 0.5f);
	glPushMatrix();
	glTranslatef(5, -35, 0);
	glTranslatef(0, 15, 0);
	glRotatef(rad, 1, 0, 0);
	glTranslatef(0, -15, 0);
	glScalef(0.5, 2, 0.5);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, -35, 0);
	glTranslatef(0, 15, 0);
	glRotatef(-rad, 1, 0, 0);
	glTranslatef(0, -15, 0);
	glScalef(0.5, 2, 0.5);
	glutSolidCube(10);
	glPopMatrix();
}

GLvoid RobotHand(int rad)
{
	glColor3f(0.2f, 1.0f, 0.5f);
	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -15, 0);
	glTranslatef(0, 15, 0);
	glRotatef(rad, 1, 0, 0);
	glRotatef(10, 0, 0, 1);
	glTranslatef(0, -15, 0);
	glScalef(0.5, 2, 0.5);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, 0);
	glTranslatef(0, -15, 0);
	glTranslatef(0, 15, 0);
	glRotatef(-rad, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	glTranslatef(0, -15, 0);
	glScalef(0.5, 2, 0.5);
	glutSolidCube(10);
	glPopMatrix();

}

GLvoid RobotHead()
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(10, 10, 10);
	glPopMatrix();
}