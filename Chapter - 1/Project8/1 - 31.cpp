#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>
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

static rightPos rp[2];

void SetupRC()
{
	camera.rotateEye(0, 0, 0);
	spherePos.y = 300;

	rp[0].x = -300;
	rp[0].y = 100;
	rp[0].z = 0;
	rp[0].rad = 180;

	rp[1].x = 300;
	rp[1].y = 100;
	rp[1].z = 0;
	rp[1].rad = 0;

}

static BOOL right1 = FALSE;
static BOOL right2 = FALSE;



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


GLfloat am[] = { -300, 100, 0, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat diffuseLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat specu[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray[] = { 0.25f, 0.25f, 0.25f, 0.1f };

GLfloat am1[] = { 300.0f, 100.0f, 0.0f, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt1[] = {1.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight1[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat specu1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray1[] = { 0.25f, 0.25f, 0.25f, 0.1f };


GLvoid DrawRight1(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	//glColor3f(1.0f, 0.1f, 0.1f);
	glutSolidSphere(5, 5, 5);
	glPopMatrix();
}

GLvoid DrawRight2(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	//glColor3f(0.0f, 1.0f, 0.9f);
	glutSolidSphere(5, 5, 5);
	glPopMatrix();
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	camera.drawCamera();
	DrawRight1(am[0], am[1], am[2]);
	DrawRight2(am1[0], am1[1], am1[2]);

	glPushMatrix();
	if (right1 == TRUE) {
		glEnable(GL_LIGHTING);
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
		//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
		//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specu);
		glLightfv(GL_LIGHT0, GL_POSITION, am);

		glEnable(GL_COLOR_MATERIAL);

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		// 이후에 나오는 모든 재질은 밝게 빛나는 완전 전반사 반사율을 갖는다.
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specu);
		glMateriali(GL_FRONT, GL_SHININESS, 64);

		glEnable(GL_LIGHT0);
	}
	else if(right1 == FALSE)
		glDisable(GL_LIGHT0);

	if (right2 == TRUE) {
		glEnable(GL_LIGHTING);
		//glColor3f(1.0f, 1.0f, 1.0f);
		////glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
		//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
		//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specu1);
		glLightfv(GL_LIGHT1, GL_POSITION, am1);

		glEnable(GL_COLOR_MATERIAL);

		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		// 이후에 나오는 모든 재질은 밝게 빛나는 완전 전반사 반사율을 갖는다.
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray1);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specu1);
		glMateriali(GL_FRONT, GL_SHININESS, 64);

		glEnable(GL_LIGHT1);
	}
	else if (right2 == FALSE)
		glDisable(GL_LIGHT1);
	glPopMatrix();

	

	glutSolidSphere(100, 100, 100);

	glPopMatrix();
	glutSwapBuffers();
	//	glFlush(); // 화면에 출력하기 
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

	if (AUTO == TRUE) {
		for (int i = 0; i < 2; ++i) {
			rp[i].rad = (rp[i].rad + 1) % 360;
		}
		am[0] = 300 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 300 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 300 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 300 * sin(3.141592 / 180.f * rp[1].rad);
	}

	glutTimerFunc(1, TimerFunction, 100);
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
		if (right1 == FALSE)
			right1 = TRUE;
		else
			right1 = FALSE;
		break;

	case '2':
		if (right2 == FALSE)
			right2 = TRUE;
		else
			right2 = FALSE;
		break;

	case 'r':
	case 'R':
		if (AUTO == FALSE)
			AUTO = TRUE;
		else
			AUTO = FALSE;
		break;

	case 't':
		for (int i = 0; i < 2; ++i) {
			rp[i].rad = (rp[i].rad + 10) % 360;
		}

		am[0] = 300 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 300 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 300 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 300 * sin(3.141592 / 180.f * rp[1].rad);
		break;

	case 'T':
		for (int i = 0; i < 2; ++i) {
			rp[i].rad -= 10;
			if (rp[i].rad <= 0)
				rp[i].rad = 360;
		}

		am[0] = 300 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 300 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 300 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 300 * sin(3.141592 / 180.f * rp[1].rad);
		break;

	case 'v':
		specu[0] -= 0.1f;
		specu[1] -= 0.1f;
		specu[2] -= 0.1f;
		specu[3] -= 0.1f;

		specu1[0] -= 0.1f;
		specu1[1] -= 0.1f;
		specu1[2] -= 0.1f;
		specu1[3] -= 0.1f;
		break;
	case 'V':
		specu[0] += 0.1f;
		specu[1] += 0.1f;
		specu[2] += 0.1f;
		specu[3] += 0.1f;

		specu1[0] += 0.1f;
		specu1[1] += 0.1f;
		specu1[2] += 0.1f;
		specu1[3] += 0.1f;
		break;

	case 'm':
		diffuseLight[0] -= 0.1f;
		diffuseLight[1] -= 0.1f;
		diffuseLight[2] -= 0.1f;
		diffuseLight[3] -= 0.1f;

		diffuseLight1[0] -= 0.1f;
		diffuseLight1[1] -= 0.1f;
		diffuseLight1[2] -= 0.1f;
		diffuseLight1[3] -= 0.1f;
		break;

	case 'M':
		diffuseLight[0] += 0.1f;
		diffuseLight[1] += 0.1f;
		diffuseLight[2] += 0.1f;
		diffuseLight[3] += 0.1f;

		diffuseLight1[0] += 0.1f;
		diffuseLight1[1] += 0.1f;
		diffuseLight1[2] += 0.1f;
		diffuseLight1[3] += 0.1f;
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}



static int swi = 0;
void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_SHIFT_L)
		start = TRUE;
	glutPostRedisplay();
}