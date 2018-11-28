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
static int mode = 1;


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
	Cam() // ������
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}
	void Init() // �ʱ�ȭ �Լ�
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}

	void drawCamera() // ī�޶� ��ġ�ϴ� �Լ�
	{
		//glPushMatrix();
		//glLoadIdentity(); // ������ ��ȯ ����� ������ķ� �ʱ�ȭ - �ð� ��ǥ�踦 �������� �ʱ�ȭ
		// ���� ��ȯ: ī�޶��� ��ġ ���� (�ʿ��� ���, �ٸ� ���� ���� ����)
		gluLookAt(m_Eye.x, m_Eye.y, m_Eye.z, m_Center.x, m_Center.y, m_Center.z, m_Up.x, m_Up.y, m_Up.z);

		glRotated(-m_Angle.x, 0.0, 1.0, 0.0);
		glRotated(m_Angle.y, 1.0, 0.0, 0.0);
		glRotated(m_Angle.z, 0.0, 0.0, 1.0);
		glTranslated(-m_MoveEye.x, -m_MoveEye.y, m_MoveEye.z);
		//glPopMatrix();
	}

public:
	void rotateEye(double x, double y, double z) // ī�޶� ȸ�� �Լ�
	{
		m_Angle.x += x;
		m_Angle.y += y;
		m_Angle.z += z;
	}
	void moveEye(double x, double y, double z) // ī�޶� �̵� �Լ�
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

	// ���� ����
	fopen_s(&file, filename, "rb");

	if (file == NULL) return 0;
	width = width_1;
	height = height_1;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// �������
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	// ���� PDF����
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Modulate�� ��
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	// ���� �Ӹ����� ����

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



void main(int agrc, char *argv[]) { // ������ �ʱ�ȭ �� ���� 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// ���� ���۸��� 3�������� �׸���. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// ���� ���� �ʱ�ȭ �Լ� 
	SetupRC();
	// �ʿ��� �ݹ� �Լ� ����
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // ��� �ݹ� �Լ� 
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ� �Լ�
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ� �Լ�
	glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸� �ݹ� �Լ�
	glutSpecialFunc(SpecialKeys);
	glutMainLoop(); // �̺�Ʈ ���� �����ϱ�
}
GLvoid drawPlan(GLvoid)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		//	glRotatef(30, 1, 0, 0);
		glScalef(1, 0.01, 1);
		glutSolidCube(400);
		glPopMatrix();
	}
}

GLvoid DrawCube()
{

}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	camera.drawCamera();


	glPushMatrix();
	glRotatef(rotate, 0, 1, 0);
	glColor3f(1.f, 1.f, 1.f);
	// �ؽ�ó�� ��ü�� ����
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50, -50, -50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50, 50, -50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50, 50, -50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50, -50, -50);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50, -50, 50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50, 50, 50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50, 50, 50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50, -50, 50);
	glEnd();

	//�翷��

	glColor3f(1.f, 0.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50, -50, -50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50, 50, -50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-50, 50, 50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-50, -50, 50);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(50, -50, -50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(50, 50, -50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50, 50, 50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50, -50, 50);
	glEnd();

	//���Ʒ���
	glColor3f(1.f, 1.f, 0.f);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50, -50, -50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50, -50, 50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50, -50, 50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50, -50, -50);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-50, 50, -50);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-50, 50, 50);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(50, 50, 50);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(50, 50, -50);
	glEnd();


	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
	//	glFlush(); // ȭ�鿡 ����ϱ� 
}



static float zZoom = 45;
GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(-1, -1, w, h);  // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (mode == 1) {
		//-- ������ ���� ���� �Ǵ� ���� ���� �� �� ���� �����Ѵ�. // 1. Ŭ���� ���� ����: ���� ������ ���
		gluPerspective(zZoom, 1.0, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -500.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
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

	rotate = (rotate + 1) % 360;
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

	case 'x':
		camera.rotateEye(1, 0, 0);
		break;
	case 'X':
		camera.rotateEye(-1, 0, 0);
		break;

	case 'y':
		camera.rotateEye(0, 1, 0);
		break;
	case 'Y':
		camera.rotateEye(0, -1, 0);
		break;

	case 'z':
		camera.rotateEye(0, 0, 1);
		break;
	case 'Z':
		camera.rotateEye(0, 0, -1);
		break;

		//move
	case 'w':
		camera.moveEye(0, 1, 0);
		break;
	case 'a':
		camera.moveEye(-1, 0, 0);
		break;

	case 's':
		camera.moveEye(0, -1, 0);
		break;

	case 'd':
		camera.moveEye(1, 0, 0);
		break;
	case '+':
		camera.moveEye(0, 0, 1);
		break;
	case '-':
		camera.moveEye(0, 0, -1);
		break;
	case 'i':
		camera.Init();
		break;

	case '9':
		zZoom += 1;
		Reshape(800, 600);
		break;

	case '0':
		zZoom -= 1;
		Reshape(800, 600);
		break;

	case '1':
		
		break;

	case '2':
		
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