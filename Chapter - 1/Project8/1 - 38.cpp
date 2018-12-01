#include <GL/freeglut.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <math.h>
#include "weather.h"
GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 2;


static float cycleRad = 200;


struct point_3 {
	float x = 0;
	float y = 450;
	float z = 0;
};

#define pi 3.14
//#define DEGTORAD pi/360 * 2
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15

static point_3 spherePos;
static BOOL start = FALSE;
static BOOL bCrash = FALSE;
static BOOL goaway = FALSE;
static BOOL vectorOn = FALSE;

GLdouble result[16] = { 0 };
GLdouble Identity[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateX_P3[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, cos(3 * DEGTORAD), -sin(3 * DEGTORAD), 0.0,
	0.0, sin(3 * DEGTORAD), cos(3 * DEGTORAD), 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateX_M3[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, cos(-3 * DEGTORAD), -sin(-3 * DEGTORAD), 0.0,
	0.0, sin(-3 * DEGTORAD), cos(-3 * DEGTORAD), 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateY_P3[16] = {
	cos(3 * DEGTORAD), 0.0, sin(3 * DEGTORAD), 0.0,
	0.0, 1.0, 0.0, 0.0,
	-sin(3 * DEGTORAD), 0.0, cos(3 * DEGTORAD), 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateY_M3[16] = {
	cos(-3 * DEGTORAD), 0.0, sin(-3 * DEGTORAD), 0.0,
	0.0, 1.0, 0.0, 0.0,
	-sin(-3 * DEGTORAD), 0.0, cos(-3 * DEGTORAD), 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateZ_P3[16] = {
	cos(3 * DEGTORAD), -sin(3 * DEGTORAD), 0.0, 0.0,
	sin(3 * DEGTORAD), cos(3 * DEGTORAD), 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};
GLdouble rotateZ_M3[16] = {
	cos(-3 * DEGTORAD), -sin(-3 * DEGTORAD), 0.0, 0.0,
	sin(-3 * DEGTORAD), cos(-3 * DEGTORAD), 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

GLvoid MultiMatrix(GLdouble y[16]) {
	for (int i = 0; i < 16; i++) {
		result[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[j + i * 4] = Identity[0 + i * 4] * y[0 + j] +
				Identity[1 + i * 4] * y[4 + j] +
				Identity[2 + i * 4] * y[8 + j] +
				Identity[3 + i * 4] * y[12 + j];
		}
	}

	for (int i = 0; i < 16; i++) {
		Identity[i] = result[i];
	}
}
GLfloat ctrlpoints[3][3][3] = { { { -200, 0, -200 },{ 0, 0, -200 },{ 200, 0, -200 } },{ { -200, 0, 0 },{ 0,350,0 },{ 200, 0, 0 } },{ { -200,0,200 },{ 0,0,200 },{ 200,0,200 } } };


void reset()
{
	start = FALSE;
	bCrash = FALSE;
	goaway = FALSE;
	cycleRad = 200;
	spherePos.x = 0;
	spherePos.z = 0;
	spherePos.y = 300;
}

struct DOUBLE_3D_POS
{
	double x, y, z;
};

class Cam
{
private:
	DOUBLE_3D_POS m_Eye = {};
	DOUBLE_3D_POS m_Center = {};
	DOUBLE_3D_POS m_Up = {};
	DOUBLE_3D_POS m_MoveEye = {};
	DOUBLE_3D_POS m_Angle = {};

public:
	Cam() // 생성자
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}
	void Init() // 초기화 함수
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}

	void drawCamera() // 카메라를 배치하는 함수
	{
		//glPushMatrix();
		//glLoadIdentity(); // 현재의 변환 행렬을 단위행렬로 초기화 - 시각 좌표계를 원점으로 초기화
		// 관측 변환: 카메라의 위치 설정 (필요한 경우, 다른 곳에 설정 가능)
		gluLookAt(m_Eye.x, m_Eye.y, m_Eye.z, m_Center.x, m_Center.y, m_Center.z, m_Up.x, m_Up.y, m_Up.z);

		glRotated(-m_Angle.x, 0.0, 1.0, 0.0);
		glRotated(m_Angle.y, 1.0, 0.0, 0.0);
		glRotated(m_Angle.z, 0.0, 0.0, 1.0);
		glTranslated(-m_MoveEye.x, -m_MoveEye.y, m_MoveEye.z);
		//glPopMatrix();
	}

public:
	void rotateEye(double x, double y, double z) // 카메라 회전 함수
	{
		m_Angle.x += x;
		m_Angle.y += y;
		m_Angle.z += z;
	}
	void moveEye(double x, double y, double z) // 카메라 이동 함수
	{
		m_MoveEye.x += x;
		m_MoveEye.y += y;
		m_MoveEye.z += z;
	}
};

static int count = 0;
static Cam camera;
void Mouse(int button, int state, int x, int y);

struct rightPos {
	float x;
	float y;
	float z;

	int rad = 0;
};





GLuint LoadTexture(const char * filename, int width_1, int height_1)
{
	GLuint texture;
	int width, height;
	unsigned char * data;
	FILE * file;

	// 파일 열기
	fopen_s(&file, filename, "rb");

	if (file == NULL) return 0;
	width = width_1;
	height = height_1;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// 색상결정
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	// 이후 PDF동일
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Modulate로 함
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	// 나는 밉맵으로 만듬

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}
GLuint texture[6];
GLubyte *pBytes;
BITMAPINFO *info;

GLubyte * TexBits;
void SetupRC()
{
	camera.rotateEye(30, 0, 0);
	camera.rotateEye(0, 0, 30);

	texture[0] = LoadTexture("texture1.bmp", 100, 100);
	texture[1] = LoadTexture("texture2.bmp", 100, 100);
	texture[2] = LoadTexture("texture3.bmp", 100, 100);
	texture[3] = LoadTexture("texture4.bmp", 100, 100);
	texture[4] = LoadTexture("texture5.bmp", 100, 100);
	texture[5] = LoadTexture("texture6.bmp", 100, 100);
}

static int rotate = 0;



void main(int agrc, char *argv[]) { // 윈도우 초기화 및 생성 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// 더블 버퍼링과 3차원으로 그린다. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// 상태 변수 초기화 함수 
	SetupRC();
	// 필요한 콜백 함수 설정
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수
	glutSpecialFunc(SpecialKeys);
	glutMainLoop(); // 이벤트 루프 실행하기
}

GLvoid DrawCube()
{

}

float alpha[9] = { 0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4 };
int selected = 0;

GLvoid Teapot(int a)
{
	glPushMatrix();
	{
		glColor4f(1.0, 0.0, 0.0, alpha[a]);
		glTranslatef(-10.f, 0.f, 0.f);
		glutSolidTeapot(30.f);
	}
	glPopMatrix();
}
GLvoid Sphere(int a)
{
	glPushMatrix();
	{
		glColor4f(0.0, 0.0, 1.0, alpha[a]);
		glTranslatef(4.f, -4.f, 0.f);
		glutSolidSphere(30.f, 24, 24);
	}
	glPopMatrix();
}
GLvoid Cube(int a)
{
	glPushMatrix();
	{
		glColor4f(0.0, 1.0, 0.0, alpha[a]);
		glTranslatef(2.f, 13.f, 0.f);
		glutSolidCube(40.f);
	}
	glPopMatrix();
}

GLvoid Models(GLvoid)
{
	glPushMatrix();
	{
		glBlendFunc(GL_ONE, GL_ZERO);
		glTranslatef(-220.f, 220.f, 10.f);

		if (selected == 0)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(0);
		Sphere(0);
		Cube(0);

		char *string = "GL_ONE, GL_ZERO";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_ZERO, GL_ONE);
		glTranslatef(0.f, 220.f, 10.f);
		if (selected == 1)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(1);
		Sphere(1);
		Cube(1);

		char *string = "GL_ZERO, GL_ONE";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_ONE, GL_ONE);
		glTranslatef(220.f, 220.f, 10.f);

		if (selected == 2)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(2);
		Sphere(2);
		Cube(2);

		char *string = "GL_ONE, GL_ONE";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(-220.f, 0.f, 10.f);

		if (selected == 3)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(3);
		Sphere(3);
		Cube(3);

		char *string = "GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glTranslatef(0.f, 0.f, 10.f);

		if (selected == 4)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(4);
		Sphere(4);
		Cube(4);

		char *string = "GL_SRC_ALPHA, GL_ONE";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(220.f, 0.f, 10.f);

		if (selected == 5)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(5);
		Sphere(5);
		Cube(5);

		char *string = "GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		glTranslatef(-220.f, -220.f, 10.f);

		if (selected == 6)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(6);
		Sphere(6);
		Cube(6);

		char *string = "GL_ONE_MINUS_DST_COLOR, GL_ZERO";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
		glTranslatef(0.f, -220.f, 10.f);

		if (selected == 7)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(7);
		Sphere(7);
		Cube(7);

		char *string = "GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glBlendFunc(GL_ONE, GL_ONE);
		glTranslatef(220.f, -220.f, 10.f);

		if (selected == 8)
		{
			glColor3f(1.f, 1.f, 1.f);
			glBegin(GL_LINE_STRIP);
			{
				glVertex3f(-50, 50, 0);
				glVertex3f(50, 50, 0);
				glVertex3f(50, -50, 0);
				glVertex3f(-50, -50, 0);
				glVertex3f(-50, 50, 0);
			}
			glEnd();
		}

		Teapot(8);
		Sphere(8);
		Cube(8);

		char *string = "GL_ONE, GL_ONE";
		glRasterPos2f(-(int)strlen(string) * 3, -65); // 문자 출력할 위치 설정
		int len = (int)strlen(string);
		for (int i = 0; i < len; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
	}
	glPopMatrix();
}
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_BLEND);

	Models();


	glutSwapBuffers();
	//   glFlush(); // 화면에 출력하기 
}

static float zZoom = 45;
GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(-1, -1, w, h);  // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (mode == 1) {
		//-- 투영은 직각 투영 또는 원근 투영 중 한 개를 설정한다. // 1. 클리핑 공간 설정: 원근 투영인 경우
		gluPerspective(zZoom, 1.0, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else if (mode == 2)
		glOrtho(-400.0, 400.0, -300.0, 300.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

static int sphereRad = 0;

static BOOL AUTO = FALSE;


void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(100, TimerFunction, 100);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'k':
		if (mode == 1) {
			mode = 2;
			Reshape(800, 600);
		}
		else if (mode == 2) {
			mode = 1;
			Reshape(800, 600);
		}
		break;

	case '+':
		alpha[selected] += 0.1f;
		break;
	case '-':
		alpha[selected] -= 0.1f;
		break;
	case 'i':
		camera.Init();
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		selected = key - 49;
		break;


	case 'r':
	case 'R':

		break;

	case 't':


		break;

	case 'T':

		break;

	case '.':

		break;

	case 'v':

		break;
	case 'V':

		break;

	case 'm':

		break;

	case 'M':

		break;

	case 'n':

		break;

	case 'N':

		break;

	case 'p':
	case 'P':

		break;

	case 'O':

		break;

	case 'o':

		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}



void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) {

	glutPostRedisplay();
}