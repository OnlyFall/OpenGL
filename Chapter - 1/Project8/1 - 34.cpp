#include <GL/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <math.h>
#include "weather.h"
#include "Robot.h"
#include "effect.h"

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 1;


static float cycleRad = 200;
static Effect effect[10];

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

static rightPos rp[2];
static rightPos moon;

GLfloat am[] = { -50, 100, 0, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt[] = { 0.6f, 0.5f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.6f, 0.5f, 0.2f, 1.0f };
GLfloat specu[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray[] = { 0.25f, 0.25f, 0.25f, 0.1f };

GLfloat am1[] = { 50.0f, 100.0f, 0.0f, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat specu1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray1[] = { 0.25f, 0.25f, 0.25f, 0.1f };

//GLfloat am1[] = { 50.0f, 100.0f, 0.0f, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat bottomAmbientLignt1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat bottomDiffuseLight1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat bottomSpecu1[400][400][4] = { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat bottomGray1[] = { 0.25f, 0.25f, 0.25f, 0.1f };

GLfloat am3[] = { robot.x, robot.y + 90, robot.z, 1.0f };
GLfloat ambientLignt3[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight3[] = { 1.0f, 0.0f, 1.0f, 1.0f };
GLfloat specu3[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray3[] = { 0.25f, 0.25f, 0.25f, 0.1f };

void SetupRC()
{
	robot.x = -75;
	camera.rotateEye(0, 0, 0);
	spherePos.y = 300;

	rp[0].y = 50;
	rp[0].rad = 180;
	rp[0].x = 100 * cos(3.141592 / 180.f * rp[0].rad);
	rp[0].z = 100 * sin(3.141592 / 180.f * rp[0].rad);

	am[0] = rp[0].x;
	am[1] = rp[0].y;
	am[2] = rp[0].z;

	rp[1].y = 50;
	rp[1].rad = 0;
	rp[1].x = 100 * cos(3.141592 / 180.f * rp[1].rad);
	rp[1].z = 100 * sin(3.141592 / 180.f * rp[1].rad);

	am1[0] = rp[1].x;
	am1[1] = rp[1].y;
	am1[2] = rp[1].z;

	moon.rad = 0;
	moon.x = 50;
	moon.y = 50;
	moon.z = 0;

	for (int i = 0; i < 100; ++i) {
		snow[i].x = rand() % 400 - 200;
		snow[i].y = rand() % 200 + 100;
		snow[i].z = rand() % 400 - 200;
	}

}

static BOOL right1 = FALSE;
static BOOL right2 = FALSE;


void SpecialKeys(int key, int x, int y);
static int seeDir = 0;
static int roboRad = 0;

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
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(1, TimerFunction, 1); // 타이머 콜백 함수
	glutSpecialFunc(SpecialKeys);
	glutMainLoop(); // 이벤트 루프 실행하기
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

GLvoid DrawMoon(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(15, 15, 15);
	glPopMatrix();
}

GLvoid drawPyramid(float x, float z)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CCW);

	if (vectorOn == FALSE) {
		glEnable(GL_NORMALIZE);
		glEnable(GL_AUTO_NORMAL);
	}

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(x - 25, 5, z + 25);
	glVertex3f(x - 25, 5, z - 25);
	glVertex3f(x + 25, 5, z - 25);
	glVertex3f(x + 25, 5, z + 25);
	glEnd();
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();

	glBegin(GL_POLYGON);
	if (vectorOn == FALSE)
		glNormal3f((0 * -50) - (-50 * 45), (0 * -50) - (-50 * 25), (0 * 45) - (0 * 25));
	glVertex3f(x + 25, 5, z - 25);
	glVertex3f(x + 25, 5, z + 25);
	glVertex3f(x, 50, z);
	glEnd();

	glBegin(GL_POLYGON);
	if (vectorOn == FALSE)
		glNormal3f((0 * 25) - (50 * 45), (0 * 25) - (50 * -25), (0 * 45) - (0 * -25));
	glVertex3f(x - 25, 5, z + 25);
	glVertex3f(x - 25, 5, z - 25);
	glVertex3f(x, 50, z);
	glEnd();


	glBegin(GL_POLYGON);
	if (vectorOn == FALSE)
		glNormal3f((0 * 25) - (0 * 45), (-50 * 25) - (0 * -25), (-50 * 45) - (0 * -25));
	glVertex3f(x + 25, 5, z - 25);
	glVertex3f(x - 25, 5, z - 25);
	glVertex3f(x, 50, z);
	glEnd();

	glBegin(GL_POLYGON);
	if (vectorOn == FALSE)
		glNormal3f((0 * -25) - (0 * 45), (50 * -25) - (0 * 25), (50 * 45) - (0 * 25));
	glVertex3f(x - 25, 5, z + 25);
	glVertex3f(x + 25, 5, z + 25);
	glVertex3f(x, 50, z);
	glEnd();
	glPopMatrix();
}

GLvoid DrawBottom(GLfloat x, GLfloat z)
{

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(x - 1, 0, z + 1);
	glVertex3f(x + 1, 0, z + 1);
	glVertex3f(x + 1, 0, z - 1);
	glVertex3f(x - 1, 0, z - 1);
	glEnd();

}

GLvoid drawEffect()
{
	for (int i = 0; i < 10; ++i) {
		glPushMatrix();
		glTranslatef(effect[i].x, effect[i].y, effect[i].z);
		glutSolidSphere(5, 5, 5);
		glPopMatrix();
	}
}


static BOOL snowOn = TRUE;
static BOOL culling = TRUE;
static BOOL crash = FALSE;

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);



	glPushMatrix();
	camera.drawCamera();
	DrawMoon(moon.x, moon.y, moon.z);
	DrawRight1(am[0], am[1], am[2]);
	DrawRight2(am1[0], am1[1], am1[2]);

	glPushMatrix();
	if (right1 == TRUE) {
		//glEnable(GL_AUTO_NORMAL);
		glEnable(GL_LIGHTING);
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
		//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
		//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLignt);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specu);
		glLightfv(GL_LIGHT0, GL_POSITION, am);



		glEnable(GL_LIGHT0);
	}
	else if (right1 == FALSE)
		glDisable(GL_LIGHT0);

	if (right2 == TRUE) {
		glEnable(GL_LIGHTING);
		//glColor3f(1.0f, 1.0f, 1.0f);
		////glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
		//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
		//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
		//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLignt1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specu1);
		glLightfv(GL_LIGHT1, GL_POSITION, am1);

		//glEnable(GL_COLOR_MATERIAL);

		//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		//// 이후에 나오는 모든 재질은 밝게 빛나는 완전 전반사 반사율을 갖는다.
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray1);
		////glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLignt1);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, specu1);
		//glMateriali(GL_FRONT, GL_SHININESS, 64);

		glEnable(GL_LIGHT1);
	}
	else if (right2 == FALSE)
		glDisable(GL_LIGHT1);
	glPopMatrix();



	//drawPlan();

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// 이후에 나오는 모든 재질은 밝게 빛나는 완전 전반사 반사율을 갖는다.
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLignt);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specu);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	drawPyramid(-150, -150);
	drawPyramid(150, 150);
	drawPyramid(150, -150);
	drawPyramid(-150, 150);
	drawPyramid(0, 0);

	if (crash == TRUE)
		drawEffect();
	glDisable(GL_COLOR_MATERIAL);




	int tmpx = 0;
	int tmpy = 0;
	for (int i = -200; i <= 200; i += 2) {
		for (int j = -200; j <= 200; j += 2) {
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
			glMaterialfv(GL_FRONT, GL_EMISSION, bottomSpecu1[tmpx][tmpy]);
			glMateriali(GL_FRONT, GL_SHININESS, 64);
			tmpy++;
			DrawBottom(i, j);
			glDisable(GL_COLOR_MATERIAL);
		}
		tmpx++;
	}
	tmpx = 0;
	tmpy = 0;

	if (snowOn == TRUE)
		wearherSnow();
	
	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specu3);
	glLightfv(GL_LIGHT2, GL_POSITION, am3);

	GLfloat direction[] = { 0, -1, 0 };
	GLfloat temp = 45;
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.f);

	glEnable(GL_LIGHT2);


	glTranslatef(robot.x, robot.y, robot.z);
	glRotatef(seeDir, 0, 1, 0);
	//로봇 그리기 장소
	RobotHead();
	RobotMiddle();
	RobotLeg(roboRad);
	RobotHand(roboRad);
	glPopMatrix();

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


static BOOL roboR = FALSE;

void TimerFunction(int value)
{
	glutPostRedisplay();

	if (AUTO == TRUE) {

		/*	for (int i = 0; i < 2; ++i) {
				rp[i].rad = (rp[i].rad + 1) % 360;
			}
			am[0] = 50 * cos(3.141592 / 180.f * rp[0].rad);
			am[2] = 50 * sin(3.141592 / 180.f * rp[0].rad);

			am1[0] = 50 * cos(3.141592 / 180.f * rp[1].rad);
			am1[2] = 50 * sin(3.141592 / 180.f * rp[1].rad);*/

		moon.rad = (moon.rad + 1) % 360;

		moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
		moon.z = 50 * sin(3.141592 / 180.f * moon.rad);
	}

	if (snowOn == TRUE) {
		for (int i = 0; i < 100; ++i) {
			snow[i].y -= 2;
			if (snow[i].y < 0) {
				snow[i].y = rand() % 300 + 100;
				snow[i].x = rand() % 400 - 200;
				snow[i].z = rand() % 400 - 200;

				int tmpx = snow[i].x + 200;
				int tmpz = snow[i].z + 200;

				if (bottomSpecu1[tmpx][tmpz][0] < 1.0f) {
					bottomSpecu1[tmpx][tmpz][0] += 0.1f;
					bottomSpecu1[tmpx][tmpz][1] += 0.1f;
					bottomSpecu1[tmpx][tmpz][2] += 0.1f;
					bottomSpecu1[tmpx][tmpz][3] += 0.1f;
				}
			}
		}
	}

	if (roboR == FALSE) {
		roboRad += 1;
		if (roboRad >= 20)
			roboR = TRUE;
	}

	else if (roboR == TRUE) {
		roboRad -= 1;
		if (roboRad <= -20)
			roboR = FALSE;
	}

	if (crash == TRUE) {
		if (effect[0].dir == 1) {
			for (int i = 0; i < 10; ++i) {
				effect[i].x += 5;
				effect[i].y = cos(3.141592 / 180.f * (36 * i)) * effect[0].count + 50;
				effect[i].z = sin(3.141592 / 180.f * (36 * i)) * effect[0].count;
			}
			effect[0].count += 1;

			if (effect[0].count == 100) {
				crash = FALSE;
				effect[0].count = 0;
			}
		}

		else if (effect[0].dir == 2) {
			for (int i = 0; i < 10; ++i) {
				effect[i].x -= 5;
				effect[i].y = cos(3.141592 / 180.f * (36 * i)) * effect[0].count + 50;
				effect[i].z = sin(3.141592 / 180.f * (36 * i)) * effect[0].count;
			}
			effect[0].count += 1;

			if (effect[0].count == 100) {
				crash = FALSE;
				effect[0].count = 0;
			}
		}

		else if (effect[0].dir == 3) {
			for (int i = 0; i < 10; ++i) {
				effect[i].x = sin(3.141592 / 180.f * (36 * i)) * effect[0].count;
				effect[i].y = cos(3.141592 / 180.f * (36 * i)) * effect[0].count + 50;
				effect[i].z -= 5;
			}
			effect[0].count += 1;

			if (effect[0].count == 100) {
				crash = FALSE;
				effect[0].count = 0;
			}
		}

		else if (effect[0].dir == 4) {
			for (int i = 0; i < 10; ++i) {
				effect[i].x = sin(3.141592 / 180.f * (36 * i)) * effect[0].count;
				effect[i].y = cos(3.141592 / 180.f * (36 * i)) * effect[0].count + 50;
				effect[i].z += 5;
			}
			effect[0].count += 1;

			if (effect[0].count == 100) {
				crash = FALSE;
				effect[0].count = 0;
			}
		}
	}

	am3[0] = robot.x;
	am3[1] = robot.y + 70;
	am3[2] = robot.z;

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
		moon.rad = (moon.rad + 10) % 360;

		moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
		moon.z = 50 * sin(3.141592 / 180.f * moon.rad);

		//for (int i = 0; i < 2; ++i) {
		//	rp[i].rad = (rp[i].rad + 10) % 360;
		//}

	/*	am[0] = 50 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 50 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 50 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 50 * sin(3.141592 / 180.f * rp[1].rad);*/

		break;

	case 'T':
		moon.rad -= 10;
		if (moon.rad <= 0)
			moon.rad = 360;

		moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
		moon.z = 50 * sin(3.141592 / 180.f * moon.rad);

		//for (int i = 0; i < 2; ++i) {
		//	rp[i].rad -= 10;
		//	if (rp[i].rad <= 0)
		//		rp[i].rad = 360;
		//}

	/*	am[0] = 50 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 50 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 50 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 50 * sin(3.141592 / 180.f * rp[1].rad);*/
		break;

	case '.':
		for (int i = 0; i < 2; ++i) {
			rp[i].rad -= 10;
			if (rp[i].rad <= 0)
				rp[i].rad = 360;
		}

		am[0] = 100 * cos(3.141592 / 180.f * rp[0].rad);
		am[2] = 100 * sin(3.141592 / 180.f * rp[0].rad);

		am1[0] = 100 * cos(3.141592 / 180.f * rp[1].rad);
		am1[2] = 100 * sin(3.141592 / 180.f * rp[1].rad);
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

	case 'n':
		ambientLignt[0] -= 0.1f;
		ambientLignt[1] -= 0.1f;
		ambientLignt[2] -= 0.1f;
		ambientLignt[3] -= 0.1f;

		ambientLignt1[0] -= 0.1f;
		ambientLignt1[1] -= 0.1f;
		ambientLignt1[2] -= 0.1f;
		ambientLignt1[3] -= 0.1f;
		break;

	case 'N':
		ambientLignt[0] += 0.1f;
		ambientLignt[1] += 0.1f;
		ambientLignt[2] += 0.1f;
		ambientLignt[3] += 0.1f;

		ambientLignt1[0] += 0.1f;
		ambientLignt1[1] += 0.1f;
		ambientLignt1[2] += 0.1f;
		ambientLignt1[3] += 0.1f;
		break;

	case 'p':
	case 'P':
		if (vectorOn == FALSE)
			vectorOn = TRUE;
		else
			vectorOn = FALSE;
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



static int swi = 0;
void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		if (robot.z > -180)
			robot.z -= 1;
		if (robot.z < -125 && robot.x > 125) {
			robot.z += 1;
			crash = TRUE;
			effect[0].dir = 4;

			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		if (robot.x < -125 && robot.z < -125) {
			robot.z += 1;
			crash = TRUE;
			effect[0].dir = 4;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		if (robot.x > -25 && robot.x < 25 && robot.z > -25 && robot.z < 25) {
			robot.z += 4;
			crash = TRUE;
			effect[0].dir = 4;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		seeDir = 0;
	}

	if (key == GLUT_KEY_DOWN) {
		if (robot.z < 180) {
			robot.z += 1;
			if (robot.z > 125 && robot.x < -125) {
				robot.z -= 1;
				effect[0].dir = 3;
				if (effect[0].count == 0) {
					for (int i = 0; i < 10; ++i) {
						effect[i].x = robot.x;
						effect[i].y = robot.y + 50;
						effect[i].z = robot.z;
					}
				}
			}
			if (robot.x > 125 && robot.z > 125) {
				robot.z -= 1;
				crash = TRUE;
				effect[0].dir = 3;
				if (effect[0].count == 0) {
					for (int i = 0; i < 10; ++i) {
						effect[i].x = robot.x;
						effect[i].y = robot.y + 50;
						effect[i].z = robot.z;
					}
				}
			}

			if (robot.x > -25 && robot.x < 25 && robot.z > -25 && robot.z < 25) {
				robot.z -= 3;
				crash = TRUE;
				effect[0].dir = 3;
				if (effect[0].count == 0) {
					for (int i = 0; i < 10; ++i) {
						effect[i].x = robot.x;
						effect[i].y = robot.y + 50;
						effect[i].z = robot.z;
					}
				}
			}

			seeDir = 180;

		}
	}


	if (key == GLUT_KEY_LEFT) {
		if (robot.x > -180)
			robot.x -= 1;
		if (robot.z > 125 && robot.x < -125) {
			robot.x += 1;
			crash = TRUE;
			effect[0].dir = 1;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}
		if (robot.x < -125 && robot.z < -125) {
			robot.x += 1;
			crash = TRUE;
			effect[0].dir = 1;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		if (robot.x > -25 && robot.x < 25 && robot.z > -25 && robot.z < 25) {
			robot.x += 1;
			crash = TRUE;
			effect[0].dir = 1;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		seeDir = 270;
	}

	if (key == GLUT_KEY_RIGHT) {
		if (robot.x < 180)
			robot.x += 1;
		if (robot.x > 125 && robot.z > 125) {
			robot.x -= 1;
			crash = TRUE;
			effect[0].dir = 2;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}
		if (robot.z < -125 && robot.x > 125) {
			robot.x -= 1;
			crash = TRUE;
			effect[0].dir = 2;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		if (robot.x > -25 && robot.x < 25 && robot.z > -25 && robot.z < 25) {
			robot.x -= 1;
			crash = TRUE;
			effect[0].dir = 2;
			if (effect[0].count == 0) {
				for (int i = 0; i < 10; ++i) {
					effect[i].x = robot.x;
					effect[i].y = robot.y + 50;
					effect[i].z = robot.z;
				}
			}
		}

		seeDir = 90;
	}
	glutPostRedisplay();
}