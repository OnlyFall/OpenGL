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
		0, 0, 0, //방향 center
		0, 1, 0); //위쪽방향(건들 ㄴㄴ) up
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



void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		camera.x += 4;
		break;
	case 'X':
		camera.x -= 4;
		break;
	case 'y':
		camera.y += 4;
		break;
	case 'Y':
		camera.y -= 4;
		break;
	case 'z':
		camera.z += 4;
		break;
	case 'Z':
		camera.z -= 4;
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

	case 'i':

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