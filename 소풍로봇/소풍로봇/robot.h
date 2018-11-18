#pragma once
#pragma once
#include <gl/freeglut.h>

struct Robot {
	float x;
	float y = 50;
	float z;

	float roboRad = 0;
	int seeDir;

	float rad;

	BOOL walk = FALSE;
};

static Robot robot[2];
static BOOL crashCheck = FALSE;

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
	glRotatef(-rad, 1, 0, 0);
	glRotatef(10, 0, 0, 1);
	glTranslatef(0, -15, 0);
	glScalef(0.5, 2, 0.5);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 0, 0);
	glTranslatef(0, -15, 0);
	glTranslatef(0, 15, 0);
	glRotatef(rad, 1, 0, 0);
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

GLfloat cs[3][3][3] = { { { -200, 0, -200 },{ 0, 0, -200 },{ 200, 0, -200 } },{ { -200, 0, 0 },{ 0,350,0 },{ 200, 0, 0 } },{ { -200,0,200 },{ 0,0,200 },{ 200,0,200 } } };


GLvoid korea(int i) // ±¹»Í!
{
	//ctrlpoints[3][3][3] = { { { robot[i].x - 10, robot[i].y + 20 , robot[i].z },{ robot[i].x, robot[i].y + 20, robot[i].z },{ robot[i].x + 10, robot[i].y + 20, robot[i].z } },{ { -200, 0, 0 },{ 0,350,0 },{ 200, 0, 0 } },{ { -200,0,200 },{ 0,0,200 },{ 200,0,200 } } };
	cs[0][0][0] = robot[i].x - 10;
	cs[0][0][1] = robot[i].y;
	cs[0][0][2] = robot[i].z - 10;

	cs[1][0][0] = robot[i].x;
	cs[1][0][1] = robot[i].y;
	cs[1][0][2] = robot[i].z - 10;

	cs[2][0][0] = robot[i].x + 10;
	cs[2][0][1] = robot[i].y;
	cs[2][0][2] = robot[i].z - 10;

	cs[0][1][0] = robot[i].x - 10;
	cs[0][1][1] = robot[i].y - 10;
	cs[0][1][2] = robot[i].z - 10;

	cs[1][1][0] = robot[i].x;
	cs[1][1][1] = robot[i].y - 10;
	cs[1][1][2] = robot[i].z - 10;

	cs[2][1][0] = robot[i].x + 10;
	cs[2][1][1] = robot[i].y - 10;
	cs[2][1][2] = robot[i].z - 10;

	cs[0][2][0] = robot[i].x - 10;
	cs[0][2][1] = robot[i].y - 20;
	cs[0][2][2] = robot[i].z - 10;

	cs[1][2][0] = robot[i].x;
	cs[1][2][1] = robot[i].y - 20;
	cs[1][2][2] = robot[i].z - 10;

	cs[2][2][0] = robot[i].x + 10;
	cs[2][2][1] = robot[i].y - 20;
	cs[2][2][2] = robot[i].z - 10;
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &cs[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);

	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
	glDisable(GL_MAP2_VERTEX_3);
}