#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <stdio.h>
#include "KeyBoard.h"

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

#define Max 360
#define MAXPLANET 3

struct Planet {
	float x;
	float y;
	float z;

	float R;
	float G;
	float B;

	int count;
	int speed;
	int position = 0;

	float smallX;
	float smallY;
	float smallZ;

	int smallcount;
	int smallspeed;
	int smallposition = 0;
};

struct Vertex {
	float x;
	float y;
	float z;
};

struct Camera {
	int xRad = -30;
	int yRad;
	int zRad;

	float x;
	float y;
	float z;

	BOOL xBOOL = FALSE;
	int xDir;
	BOOL yBOOL = FALSE;
	int yDir;
	BOOL zBOOL = FALSE;
	int zDir;
};

static Camera camera;
static Planet planet[3];
static Vertex vertex[Max];
void SetupRC()
{
	// 필요한 변수들, 좌표값 등의 초기화 // 기능 설정 초기화
}


void main(int agrc, char *argv[]) { // 윈도우 초기화 및 생성 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// 더블 버퍼링과 3차원으로 그린다. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// 상태 변수 초기화 함수 
	SetupRC();
	for (int i = 0; i < Max; ++i) {
		vertex[i].x = cos((3.141592 / 180.f) * i) * 100;
		vertex[i].y = sin((3.141592 / 180.f) * i) * 100;
	}
	for (int i = 0; i < MAXPLANET; ++i) {
		planet[i].speed = i * 3 + 2;
		planet[i].smallspeed = i + 2;
	}
	// 필요한 콜백 함수 설정
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수
	glutMainLoop(); // 이벤트 루프 실행하기
}

GLvoid FirstPlanet()
{
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 1, 0);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x, vertex[i].y);
		glVertex2f(vertex[i + 1].x, vertex[i + 1].y);
		glEnd();
	}
	glTranslatef(planet[0].x, planet[0].y, 0);
	glColor3f(0.3f, 0.7f, 0.4f);
	glutSolidSphere(15, 15, 15);

	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x / 2, vertex[i].y / 2);
		glVertex2f(vertex[i + 1].x / 2, vertex[i + 1].y / 2);
		glEnd();
	}
	glTranslatef(planet[0].smallX / 2, planet[0].smallY / 2, 0);
	glColor3f(0.3f, 0.1f, 0.9f);
	glutSolidSphere(7, 7, 7);
	glPopMatrix();
}

GLvoid SecondPlanet()
{
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	glRotatef(-45, 0, 1, 0);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x, vertex[i].y);
		glVertex2f(vertex[i + 1].x, vertex[i + 1].y);
		glEnd();
	}
	glTranslatef(planet[1].x, planet[1].y, 0);
	glColor3f(0.3f, 0.4f, 0.4f);
	glutSolidSphere(15, 15, 15);

	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x / 2, vertex[i].y / 2);
		glVertex2f(vertex[i + 1].x / 2, vertex[i + 1].y / 2);
		glEnd();
	}
	glTranslatef(planet[1].smallX / 2, planet[1].smallY / 2, 0);
	glColor3f(0.3f, 0.1f, 0.9f);
	glutSolidSphere(7, 7, 7);
	glPopMatrix();
}

GLvoid ThirdPlanet()
{
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x, vertex[i].y);
		glVertex2f(vertex[i + 1].x, vertex[i + 1].y);
		glEnd();
	}
	glTranslatef(planet[2].x, planet[2].y, 0);
	glColor3f(0.3f, 0.5f, 0.4f);
	glutSolidSphere(15, 15, 15);

	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < Max - 1; ++i) {
		glBegin(GL_LINES);
		glVertex2f(vertex[i].x / 2, vertex[i].y / 2);
		glVertex2f(vertex[i + 1].x / 2, vertex[i + 1].y / 2);
		glEnd();
	}
	glTranslatef(planet[2].smallX / 2, planet[2].smallY / 2, 0);
	glColor3f(0.3f, 0.1f, 0.9f);
	glutSolidSphere(7, 7, 7);
	glPopMatrix();
}

GLvoid MainPlanet()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(30, 30, 30);	
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glRotatef(camera.xRad, 1, 0, 0);
	glRotatef(camera.yRad, 0, 1, 0);
	glRotatef(camera.zRad, 0, 0, 1);

	glTranslatef(camera.x, camera.y, camera.z);
	MainPlanet();
	FirstPlanet();
	SecondPlanet();
	ThirdPlanet();
	glPopMatrix();


	glutSwapBuffers();
	//	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(0, 0, w, h);  // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//-- 투영은 직각 투영 또는 원근 투영 중 한 개를 설정한다. // 1. 클리핑 공간 설정: 원근 투영인 경우
	gluPerspective(45.0f, 1.0, 1.0, 1000.0);
	glTranslatef(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

static int count = 0;

void TimerFunction(int value)
{
	glutPostRedisplay();

	for (int i = 0; i < MAXPLANET; ++i) {
		if (planet[i].count == 0) {
			if (planet[i].position < 359)
				planet[i].position++;
			else
				planet[i].position = 0;

			planet[i].x = vertex[planet[i].position].x;
			planet[i].y = vertex[planet[i].position].y;
		}
		planet[i].count = (planet[i].count + 1) % planet[i].speed;

		if (planet[i].smallcount == 0) {
			if (planet[i].smallposition < 359)
				planet[i].smallposition++;
			else
				planet[i].smallposition = 0;
			planet[i].smallX = vertex[planet[i].smallposition].x;
			planet[i].smallY = vertex[planet[i].smallposition].y;
		}
		planet[i].smallcount = (planet[i].smallcount + 1) % planet[i].smallspeed;
	}
	
	if (camera.xBOOL == TRUE) {
		if (camera.xDir == 1)
			camera.xRad = (camera.xRad + 1) % 360;
		else if (camera.xDir == 2)
			camera.xRad = (camera.xRad - 1) % 360;
	}

	if (camera.yBOOL == TRUE) {
		if (camera.yDir == 1)
			camera.yRad = (camera.yRad + 1) % 360;
		else if (camera.yDir == 2)
			camera.yRad = (camera.yRad - 1) % 360;
	}

	if (camera.zBOOL == TRUE) {
		if (camera.zDir == 1)
			camera.zRad = (camera.zRad + 1) % 360;
		else if (camera.zDir == 2)
			camera.zRad = (camera.zRad - 1) % 360;
	}

	glutTimerFunc(10, TimerFunction, 1);
}


static BOOL check = FALSE;

void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		if (camera.xBOOL == FALSE) {
			camera.xBOOL = TRUE;
			camera.xDir = 1;
		}
		else if (camera.xBOOL == TRUE)
			camera.xBOOL = FALSE;
		break;

	case 'X':
		if (camera.xBOOL == FALSE) {
			camera.xBOOL = TRUE;
			camera.xDir = 2;
		}
		else if (camera.xBOOL == TRUE)
			camera.xBOOL = FALSE;
		break;

	case 'y':
		if (camera.yBOOL == FALSE) {
			camera.yBOOL = TRUE;
			camera.yDir = 1;
		}
		else if (camera.yBOOL == TRUE)
			camera.yBOOL = FALSE;
		break;

	case 'Y':
		if (camera.yBOOL == FALSE) {
			camera.yBOOL = TRUE;
			camera.yDir = 2;
		}
		else if (camera.yBOOL == TRUE)
			camera.yBOOL = FALSE;
		break;

	case 'z':
		if (camera.zBOOL == FALSE) {
			camera.zBOOL = TRUE;
			camera.zDir = 1;
		}
		else if (camera.zBOOL == TRUE)
			camera.zBOOL = FALSE;
		break;

	case 'Z':
		if (camera.zBOOL == FALSE) {
			camera.zBOOL = TRUE;
			camera.zDir = 2;
		}
		else if (camera.zBOOL == TRUE)
			camera.zBOOL = FALSE;
		break;

	case 'w':
		camera.y += 5;
		break;

	case 'a':
		camera.x -= 5;
		break;

	case 's':
		camera.y -= 5;
		break;

	case 'd':
		camera.x += 5;
		break;

	case '+':
		camera.z += 5;
		break;

	case '-':
		camera.z -= 5;
		break;

	case 'i':
		camera.xRad = -30;
		camera.yRad = 0;
		camera.zRad = 0;

		camera.x = 0;
		camera.y = 0;
		camera.z = 0;

		camera.xBOOL = FALSE;
		camera.yBOOL = FALSE;
		camera.zBOOL = FALSE;

		camera.xDir = 0;
		camera.yDir = 0;
		camera.zDir = 0;
		break;
	}
}
