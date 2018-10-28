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
static int mode = 1;

#define pi 3.14
//#define DEGTORAD pi/360 * 2
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15

double angle;
int direction;
int LoAdder;
int LoA = 0;
int LoX = 0;
int MidXA = 0;
int MidYA = 0;
int TopXA = 0;
int TopZA = 0;
int nPoint = 0;
int Point[pointArrSize][2] = { 0 }; // Shape 위치 지정
static bool DiX = false;

struct point_3 {
	float x = 0;
	float y = 0;
	float z = 60;
};

static point_3 spherePos;

typedef struct set {
	float x;
	float y;
	float z;
};

typedef struct Translate_pos {
	float x;
	float y;
	float z;
	float degree;
};

typedef struct Three {
	float size;
	float x;
	float y;
};

typedef struct Shape
{
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;

	BOOL any;

};

Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;



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

static int BallDir = 0;

static float R;
static float G;
static float B;

static int count = 0;
static int temp = 0;
static BOOL change = FALSE;
static BOOL airDir = FALSE;

static Three sq[3];

static Cam camera;

struct point {
	float x;
	float y;
};

static point PlanePosition[360];
static int nowPosition = 0;

void SetupRC()
{
	camera.Init();

	for (int i = 0; i < 360; ++i) {
		PlanePosition[i].x = cos((3.141592 / 180.f)  * (i + 1)) * 100;
		PlanePosition[i].y = sin((3.141592 / 180.f)  * (i + 1)) * 100;
	}
	nowPosition = 359;

	sq[0].size = 9;
	sq[1].size = 6;
	sq[2].size = 3;

	sq[0].x = 5;
	sq[1].x = 15;
	sq[2].x = 25;
}

const void camera_custom
(double pos_x, double pos_y, double pos_z, //위치
	double degree, const double rot_x, const double rot_y, const double rot_z, //회전
	const double move_x, const double move_y, const double move_z //움직임
) {

	EYE.x =
		((cos(rot_y) * cos(rot_z)) +
		(sin(rot_x) * sin(rot_y) * cos(rot_z) + cos(rot_x) * sin(rot_z)) +
			((((-1) * cos(rot_x)) * sin(rot_y) * cos(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.y =
		(((-1) * cos(rot_y) * sin(rot_z)) +
		(((-1) * sin(rot_x) * sin(rot_y) * sin(rot_z)) + (cos(rot_x) * cos(rot_z))) +
			((cos(rot_x) * sin(rot_y) * sin(rot_z)) + (sin(rot_x) * sin(rot_z))));

	EYE.z =
		(sin(rot_y) +
		(((-1) * sin(rot_x)) * cos(rot_y)) +
			(cos(rot_x) * cos(rot_y)));//stay



	AT.x = pos_x;
	AT.y = pos_y;
	AT.z = pos_z;

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
static int TreeSize = 30;


GLvoid Draw_Low()
{
	glPushMatrix();
	{
		glColor4f(1.0f, 0.7f, 0.1f, 1.0f);
		glScaled(1.5, 0.25, 1.0);
		glutSolidCube(ShapeSize * 2);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(ShapeSize * 2);
	}
	glPopMatrix();
}

GLvoid Draw_Top()
{
	glPushMatrix();
	{
		glColor4f(0.1f, 1.0f, 0.1f, 1.0f);
		glPushMatrix();
		glRotated(TopXA, 1.0, 0.0, 0.0);
		glRotated(TopZA, 0.0, 0.0, 1.0);
		glScaled(1.0, 4.0, 1.0);
		glTranslated(0.0, ShapeSize / 4, 0.0);
		glutSolidCube(ShapeSize / 2);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(ShapeSize / 2);

		glPopMatrix();
	}
	glPopMatrix();
}

GLvoid Draw_Mid()
{
	glPushMatrix();
	glColor4f(1.0f, 0.1f, 0.1f, 1.0f);
	glRotated(MidYA, 0.0, 1.0, 0.0);
	glRotated(MidXA, 1.0, 0.0, 0.0);
	glTranslated(0.0, ShapeSize, 0.0);
	glPushMatrix();
	glScaled(1.0, 4.0, 1.0);

	glutSolidCube(ShapeSize / 2);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutWireCube(ShapeSize / 2);

	glPopMatrix();

	glTranslated(0.0, ShapeSize, 0.0);
	Draw_Top();
	glPopMatrix();
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

static int ballRad = 0;
GLvoid Ball(GLvoid)
{
	glPushMatrix();
	{
		glTranslatef(0, 0, 0);
		glRotatef(ballRad, 1, 0, 0);
	}
	glPopMatrix();
}

static int running = 0;
static int leg = 0;
GLvoid Running(GLvoid)
{
	{
		glColor3f(0.5f, 0.7f, 0.5f);
		glBegin(GL_TRIANGLES);
		glVertex3f(-200, 3, 0);
		glVertex3f(-200, 3, 200);
		glVertex3f(0, 3, 200);
		glEnd();
	}

	glColor3f(0.1f, 0.1f, 0.9f);
	glPushMatrix();
	{
		glTranslatef(-170, 20, 170);
		glScalef(1, 0.2, 1);
		glRotatef(-running, 0, 0, 1);
		glutWireTorus(20, 30, 20, 20);

	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(-170, 30, 165);
		glTranslated(0, 30, 0);
		glScalef(0.2, 1, 1);
		glutSolidCube(20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(-170, 40, 170);
		glTranslated(0, 20, 0);
		glRotatef(leg, 0, 0, 1);
		glTranslatef(0, -20, 0);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(-170, 40, 160);
		glTranslated(0, 20, 0);
		glRotatef(-leg, 0, 0, 1);
		glTranslatef(0, -20, 0);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(20);
	}
	glPopMatrix();
}

GLvoid Tree(int size)
{
	glPushMatrix();
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_TRIANGLES);
		glVertex3f(200, 3, 0);
		glVertex3f(200, 3, 200);
		glVertex3f(0, 3, 200);
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.9f, 0.5f, 0.0f);
		glTranslatef(160, 50, 170);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(100);
	}
	glPopMatrix();

	glPushMatrix();
	{

		glTranslatef(160, 50, 170);
		glTranslatef(0, 50, 0);
		glColor3f(0.0f, 1.0f, 0.4f);
		glutWireSphere(size, 20, 20);
	}
	glPopMatrix();
}

static int hand = 0;
static int updownSize = 0;
GLvoid UpDown(GLvoid)
{
	glColor3f(0.5f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(200, 3, 0);
	glVertex3f(200, 3, -200);
	glVertex3f(0, 3, -200);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
	{
		glTranslatef(170, 10, -165);
		glScalef(1, 0.4, 0.4);
		glutSolidCube(40);

	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(170, 37 + updownSize, -170);
		glTranslated(0, 20, 0);
		glRotatef(hand, 1, 0, 0);
		glTranslatef(0, -10, 0);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(170, 37, -170);
		glTranslatef(0, -20, 0);
		glRotatef(-hand, 1, 0, 0);
		glScalef(0.2, 1, 0.2);
		glTranslatef(0, 10, 0);
		glutSolidCube(20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(170, 37 + updownSize, -160);
		glTranslated(0, 20, 0);
		glRotatef(-hand, 1, 0, 0);
		glTranslatef(0, -10, 0);
		glScalef(0.2, 1, 0.2);
		glutSolidCube(20);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(170, 37, -160);
		glTranslatef(0, -20, 0);
		glRotatef(hand, 1, 0, 0);
		glScalef(0.2, 1, 0.2);
		glTranslatef(0, 10, 0);
		glutSolidCube(20);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0f, 0.0f, 0.0);
		glTranslatef(170, 57 + updownSize, -185);
		glRotatef(90, 0, 0, 1);
		glutWireCylinder(1, 40, 20, 20);

		glTranslatef(0, 0, 40);
		glutSolidCylinder(20, 10, 20, 20);

		glTranslatef(0, 0, -50);
		glutSolidCylinder(20, 10, 20, 20);
	}
	glPopMatrix();
}

GLvoid rotatePeople(GLvoid)
{

	glColor3f(1.0f, 0.5f, 0.3f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-200, 3, 0);
	glVertex3f(-200, 3, -200);
	glVertex3f(0, 3, -200);
	glEnd();

	glColor3f(1.0f, 0.0f, 1.0f);
	glPushMatrix();
	{
		glTranslatef(-170, 50, -170);
		glRotatef(90, 1, 0, 0);
		glutSolidCylinder(2, 50, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-170, 50, -130);
		glRotatef(90, 1, 0, 0);
		glutSolidCylinder(2, 50, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-170, 50, -175);
		glutSolidCylinder(2, 50, 50, 50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.5f, 0.5f, 0.3f);
		glTranslatef(-170, 50, -150);
		glRotatef(running, 0, 0, 1);
		glTranslatef(0, -10, 0);
		glutSolidSphere(10, 10, 10);

		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(0, -20, 0);
		glScalef(0.2, 0.7, 1);
		glutSolidCube(40);

		glScalef(1, 1, 1);

		glScalef(0.3f, 1, 0.3f);
		glTranslatef(0, 30, 50);
		glutSolidCube(20);

		glTranslatef(0, 0, -100);
		glutSolidCube(20);
	}
	glPopMatrix();

}

GLvoid DrawSphere()
{
	glPushMatrix();

	//Move
	glTranslatef(spherePos.x, spherePos.y + 20, spherePos.z);
	//Rotation 행렬 곱해주기.
	glMultMatrixd(Identity);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutWireSphere(20, 20, 20);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(20, 10, 10);
	glPopMatrix();
}

static int airRad = 0;
GLvoid DrawAirplane(GLvoid)
{
	glPushMatrix();
	{
		glTranslatef(PlanePosition[nowPosition].x, 0, PlanePosition[nowPosition].y);
		glTranslatef(-10, 0, 0);
		glRotatef(-nowPosition, 0, 1, 0);
		glTranslatef(10, 0, 0);

		glPushMatrix();
		if(airDir == FALSE)
			glRotatef(90, 0, 1, 0);
		else if(airDir == TRUE)
			glRotatef(270, 0, 1, 0);

		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(0, 100, 0);
		glScalef(0.3, 0.1, 1);
		glutSolidCube(40);
		glTranslatef(0, 200, 0);
		glutSolidCube(40);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0f, 0.7f, 0.5f);
		glTranslatef(0, 110, 10);
		glScalef(0.05f, 0.1f, 0.025f);
		glutSolidCube(150);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0f, 0.7f, 0.5f);
		glTranslatef(0, 110, -10);
		glScalef(0.05f, 0.1f, 0.025f);
		glutSolidCube(150);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 0.7f, 0.5f);
		glTranslatef(-20, 110, 0);
		glScalef(1, 0.2, 0.2);
		glutSolidSphere(40, 40, 40);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(-40, 110, 0);
		glScalef(0.3, 0.1, 1);
		glutSolidCube(50);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(20, 110, 0);
		glRotatef(airRad, 1, 0, 0);
		glScalef(0.05f, 0.05f, 0.9f);
		glutSolidCube(20);
		glPopMatrix();


		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(-65, 110, 0);
		glTranslatef(-sq[0].x, 0, 0);
		glutSolidSphere(sq[0].size, sq[0].size, sq[0].size);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-65, 110, 0);
		glTranslatef(-sq[1].x, 0, 0);
		glutSolidSphere(sq[1].size, sq[1].size, sq[1].size);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-65, 110, 0);
		glTranslatef(-sq[2].x, 0, 0);
		glutSolidSphere(sq[2].size, sq[2].size, sq[2].size);
		glPopMatrix();


		glPopMatrix();
	}
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

	//=====바닥 그리기=====
	drawPlan();

	//=====구르는 공======
	DrawSphere();

	//=====크 레 인=====
	glPushMatrix();
	glTranslated(LoX, 5, 0);
	glRotated(LoA, 0.0, 1.0, 0.0);
	Draw_Low();
	Draw_Mid();
	glPopMatrix();
	//=====레몬트리======
	Tree(TreeSize);

	//=====런닝머신=====
	Running();

	//=====업다운머신=====
	UpDown();

	//=====철봉도는놈=====
	rotatePeople();

	//=====비행기=====
	DrawAirplane();

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

BOOL legBool = FALSE;
BOOL TreeBool = FALSE;
BOOL handBool = FALSE;

BOOL xBool = FALSE;
BOOL yBool = FALSE;
BOOL zBool = FALSE;

void TimerFunction(int value)
{
	glutPostRedisplay();

	if (handBool == FALSE) {
		hand += 5;
		updownSize -= 2;
		if (hand == 15)
			handBool = TRUE;
	}
	else if (handBool == TRUE) {
		hand -= 5;
		updownSize += 2;
		if (hand == 0)
			handBool = FALSE;
	}

	if (legBool == FALSE) {
		leg += 5;
		if (leg == 15)
			legBool = TRUE;
	}
	else if (legBool == TRUE) {
		leg -= 5;
		if (leg == -15)
			legBool = FALSE;
	}

	running = (running + 1) % 360;
	if (TreeBool == FALSE) {
		TreeSize += 1;
		if (TreeSize == 40)
			TreeBool = TRUE;
	}
	else {
		TreeSize -= 1;
		if (TreeSize == 20)
			TreeBool = FALSE;
	}

	if (xBool == FALSE && BallDir == 1) {
		spherePos.x += 1;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10)) {
			xBool = TRUE;
			spherePos.x -= 3;
		}
		//sphereAngle.z -= 3;
		MultiMatrix(rotateZ_P3);
		if (spherePos.x == 100)
			xBool = TRUE;
	}
	else if (xBool == TRUE && BallDir == 1) {
		spherePos.x -= 1;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10)) {
			xBool = FALSE;
			spherePos.x += 3;
		}
		//sphereAngle.z -= 3;
		MultiMatrix(rotateZ_M3);
		if (spherePos.x == -100)
			xBool = FALSE;
	}

	if (zBool == FALSE && BallDir == 2) {
		spherePos.z += 1;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10)) {
			zBool = TRUE;
			spherePos.z -= 3;
		}
		//sphereAngle.x += 3;
		MultiMatrix(rotateX_M3);
		if (spherePos.z == 100)
			zBool = TRUE;
	}
	else if (zBool == TRUE && BallDir == 2) {
		spherePos.z -= 1;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10)) {
			zBool = FALSE;
			spherePos.z += 3;
		}
		//sphereAngle.z += 3;
		MultiMatrix(rotateX_P3);
		if (spherePos.z == -100)
			zBool = FALSE;
	}


	if (!DiX) {
		LoX += 2;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10))
			DiX = !DiX;
	}
	else {
		LoX -= 2;
		if ((LoX - 15 < spherePos.x + 10) && (0 - 15 < spherePos.z + 10)
			&& (LoX + 15 > spherePos.x - 10) && (0 + 15 > spherePos.z - 10))
			DiX = !DiX;
	}
	if (LoX > 50)	DiX = !DiX;
	else if (LoX < -50)	DiX = !DiX;

	/* Low Angle Control*/
	if (LoA > 360)	LoA = 0;
	else if (LoA < 0)	LoA = 360;
	LoA += LoAdder;

	airRad = (airRad + 20) % 360;
	
	//nowPosition = (nowPosition + 1) % 360;
	if (airDir == FALSE) {
		nowPosition -= 1;
		if (nowPosition < 0)
			nowPosition = 359;
	}

	else if (airDir == TRUE) {
		nowPosition += 1;
		if (nowPosition > 359)
			nowPosition = 0;
	}


	for (int i = 0; i < 3; ++i) {
		sq[i].size -= 0.3;
		sq[i].x += 1;

		if (sq[i].size <= 0) {
			sq[i].x = 5;
			sq[i].size = 9;
		}
	}

	glutTimerFunc(100, TimerFunction, 1);

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

	case 'm':
	case 'M':
		if (airDir == FALSE)
			airDir = TRUE;
		else
			airDir = FALSE;
		break;

	case '1':
		BallDir = 1;
		break;

	case '2':
		BallDir = 2;
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