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

struct Spe {
	float x;
	float y;
	float z;
};

struct Camera {
	int xRad = 30;
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

static Spe sp;

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
static position rad;

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

static BOOL xMove = FALSE;
static BOOL yMove = FALSE;
static BOOL zMove = FALSE;
static int xDir = 0;
static int yDir = 0;
static int zDir = 0;

static int xB, yB, zB;

static int shape = 0;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glTranslatef(camera.x, camera.y, camera.z);


	glRotatef(camera.xRad, 1, 0, 0);
	glRotatef(camera.yRad, 0, 1, 0);
	glRotatef(camera.zRad, 0, 0, 1);

	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		//glRotatef(xB, 1, 0, 0);
		//glRotatef(yB, 0, 1, 0);
		//glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
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
	//	glRotatef(30, 1, 0, 0);
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
	//	glRotatef(30, 1, 0, 0);
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

		glTranslatef(0, 30, 25);
	//	glRotatef(30, 1, 0, 0);
		glScalef(0.1, 0.1, 1);
		glutSolidCube(50);
		glPopMatrix();
	}


	{
		glPushMatrix();

		if (xMove == TRUE) {
			glTranslatef(sp.x, 80, sp.z);
			glRotatef(rad.radian, 0, 0, 1);
		}
		else if (zMove == TRUE) {
			glTranslatef(sp.x, 80, sp.z);
			glRotatef(rad.radian, 1, 0, 0);
		}
		else {
			glTranslatef(sp.x, 80, sp.z);
			glRotatef(rad.radian, 0, 1, 0);
		}
		glutWireSphere(50, 20, 20);

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


	if (xMove == TRUE) {
		if (xDir == 1) {
			if(sp.x < 160)
				sp.x += 1;
			rad.radian += 10;
		}

		else if (xDir == 2) {
			if (sp.x > -160)
				sp.x -= 1;
			rad.radian -= 10;
		}
	}

	if (yMove == TRUE) {
		if (yDir == 1)
			rad.radian += 1;
		else
			rad.radian -= 1;
	}

	if (zMove == TRUE) {
		if (zDir == 2) {
			if (sp.z < 160)
				sp.z += 1;
			rad.radian += 10;
		}
		
		else if (zDir == 1) {
			if (sp.z > -160)
				sp.z -= 1;
			rad.radian -= 10;
		}
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
	glutTimerFunc(100, TimerFunction, 1);

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

	case 'l':
		if (xMove == FALSE) {
			xMove = TRUE;
			xDir = 1;
		}
		else
			xMove = FALSE;

		break;

	case 'L':
		if (xMove == FALSE) {
			xMove = TRUE;
			xDir = 2;
		}
		else
			xMove = FALSE;

		break;

	case 'm':
		if (yMove == FALSE) {
			yMove = TRUE;
			yDir = 1;
		}
		else
			yMove = FALSE;
		break;

	case 'M':
		if (yMove == FALSE) {
			yMove = TRUE;
			yDir = 2;
		}
		else
			yMove = FALSE;

		break;

	case 'n':
		if (zMove == FALSE) {
			zMove = TRUE;
			zDir = 1;
		}
		else
			zMove = FALSE;
		break;

	case 'N':
		if (zMove == FALSE) {
			zMove = TRUE;
			zDir = 2;
		}
		else
			zMove = FALSE;
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

static BOOL check = FALSE;

void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}