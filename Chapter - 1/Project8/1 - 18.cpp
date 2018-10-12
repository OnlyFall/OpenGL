#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

typedef struct Translate_pos {
	float x = 400;
	float y;
	float z;
	float degree;
};


Translate_pos camera;
struct position {
	int x;
	int y;
	int z;

	float r;
	float g;
	float b;

	int shape;
	int radian;
	GLdouble size;
};

static position type[20];

static float R;
static float G;
static float B;

static int count = 0;
static int temp = 0;
static BOOL change = FALSE;
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
	// 필요한 콜백 함수 설정
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수
	glutMainLoop(); // 이벤트 루프 실행하기
}
static int rad;
static int rad2;
static int rad3;
static int rad4;

static int xB, yB, zB;

static int shape = 0;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);




	glPushMatrix();
	gluLookAt(
		camera.x, camera.y, camera.z,  //위5 eye
		1, 0, 0, //방향 center
		0, 1, 0); //위쪽방


	glRotatef(90, 0, 1, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glScalef(1, 0.1, 1);
		glutSolidCube(400);
		glPopMatrix();
	}

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glTranslatef(25, 25, 0);
		glRotatef(30, 1, 0, 0);
		glScalef(1, 0.1, 0.1);
		glutSolidCube(50);
		glPopMatrix();
	}

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glTranslatef(0, 40, 10);
		glRotatef(30, 1, 0, 0);
		glScalef(0.1, 1, 0.1);
		glutSolidCube(50);
		glPopMatrix();
	}

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glTranslatef(0, 15, 25);
		glRotatef(30, 1, 0, 0);
		glScalef(0.1, 0.1, 1);
		glutSolidCube(50);
		glPopMatrix();
	}
	



	if (shape == 1) {

		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidCube(50);
		else
			glutWireCube(50);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireCube(50);
		else
			glutSolidCube(50);
		glPopMatrix();
	}

	else if (shape == 2) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidTeapot(50);
		else
			glutWireTeapot(50);
		glPopMatrix();
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireTeapot(50);
		else
			glutSolidTeapot(50);
		glPopMatrix();
	}

	else if (shape == 3) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidSphere(50, 8, 8);
		else
			glutWireSphere(50, 8, 8);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireSphere(50, 8, 8);
		else
			glutSolidSphere(50, 8, 8);
		glPopMatrix();
	}

	else if (shape == 4) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidCone(50, 50, 8, 8);
		else
			glutWireCone(50, 50, 8, 8);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireCone(50, 50, 8, 8);
		else
			glutSolidCone(50, 50, 8, 8);

		glPopMatrix();
	}
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

static int dir = 1;
static BOOL rad1Bool = FALSE;
static BOOL rad2Bool = FALSE;
static BOOL rad3Bool = FALSE;
static BOOL rad4Bool = FALSE;

static BOOL xBool = FALSE;
static BOOL yBool = FALSE;
static BOOL zBool = FALSE;

static BOOL rBool = FALSE;
static BOOL gBool = FALSE;
static BOOL bBool = FALSE;

void TimerFunction(int value)
{
	glutPostRedisplay();

	if (rad1Bool == TRUE)
		rad += 10;

	if (rad2Bool == TRUE)
		rad2 += 10;

	if (rad3Bool == TRUE)
		rad3 += 10;

	if (rad4Bool == TRUE)
		rad4 += 10;

	if (xBool == TRUE)
		xB = (xB + 10) % 360;

	if (yBool == TRUE)
		yB = (yB + 10) % 360;

	if (zBool == TRUE)
		zB = (zB + 10) % 360;

	if (rBool == false) {
		R += 3;
		if (R >= 252)
			rBool = TRUE;
	}
	else if (rBool == TRUE) {
		R -= 3;
		if (R <= 3)
			rBool = FALSE;
	}

	if (gBool == false) {
		G += 5;
		if (G >= 250)
			gBool = TRUE;
	}
	else if (gBool == TRUE) {
		G -= 5;
		if (G <= 5)
			gBool = FALSE;
	}

	if (bBool == false) {
		B += 1;
		if (B >= 254)
			bBool = TRUE;
	}
	else if (bBool == TRUE) {
		B -= 1;
		if (B <= 1)
			bBool = FALSE;
	}
	glutTimerFunc(100, TimerFunction, 1);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'I':
		rad = 0;
		rad2 = 0;
		rad3 = 0;
		rad4 = 0;

		rad1Bool = FALSE;
		rad2Bool = FALSE;
		rad3Bool = FALSE;
		rad4Bool = FALSE;

		R = 0;
		G = 0;
		B = 0;
		break;

	case 'L':
		if (rad2Bool == FALSE)
			rad2Bool = TRUE;
		else if (rad2Bool == TRUE)
			rad2Bool = FALSE;
		break;

	case 'R':
		if (rad3Bool == FALSE)
			rad3Bool = TRUE;
		else if (rad3Bool == TRUE)
			rad3Bool = FALSE;

		break;

	case 'O':
		if (rad1Bool == FALSE)
			rad1Bool = TRUE;
		else if (rad1Bool == TRUE)
			rad1Bool = FALSE;
		
		break;
	case 'x':
		if (xBool == FALSE)
			xBool = TRUE;
		else
			xBool = FALSE;
		break;

	case 'y':
		if (yBool == FALSE)
			yBool = TRUE;
		else
			yBool = FALSE;
		break;

	case 'z':
		if (zBool == FALSE)
			zBool = TRUE;
		else
			zBool = FALSE;
		break;

	case 'C':
		if (change == FALSE)
			change = TRUE;
		else
			change = FALSE;
		break;

	case 'i':
		camera.x += 4;
		break;

	case 'o':
		if (camera.x > 4)
			camera.x -= 4;
		else
			camera.x -= 0.5;
		break;

	case '1':
		shape = 1;
		break;

	case '2':
		shape = 2;
		break;

	case '3':
		shape = 3;
		break;

	case '4':
		shape = 4;
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}

static BOOL check = FALSE;

void Mouse(int button, int state, int x, int y)
{
	glutPostRedisplay();
}