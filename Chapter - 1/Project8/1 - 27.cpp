#include <GL/freeglut.h> 
#include <random>
#include <time.h>
#include <math.h>
GLvoid Reshape(int w, int h);

//해상도 설정
#define WideSize 800
#define HighSize 600
#define Z_Size 400

//그리기 제어
GLvoid drawScene(GLvoid);
BOOL draw_Act_On;

//타이머 제어
void Timerfunction(int value);
BOOL Time_Act_On; //타이머 활성화 여부
int move_count; //타이머 카운트

			//마우스 제어
void Motion(int x, int y);
void Mouse(int button, int state, int x, int y);
POINT ms_click; //마우스 좌표
BOOL Mouse_Act_On; //마우스 활성화 여부

			   //키보드 제어
void Keyboard(unsigned char key, int x, int y);
BOOL Keyboard_Act_On; //키보드 활성화 여부

				 //색 선정
static int BallDir = 0;

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

typedef struct Color
{
	int R;
	int G;
	int B;
};
typedef struct Translate_pos {
	float x;
	float y;
	float z;
	float degree;
};

typedef struct BoOL
{
	int b_x;
	int b_y;
	int b_z;
};

#define PT 20//도형 갯수 설정
#define PI 3.141592 //파이
#define pi 3.14
#define DEGTORAD pi/360 * 2
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15

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
typedef struct Shape
{
	Color cl;//색상
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BoOL b;
	BOOL any;

};

typedef struct QUAD
{
	Color cl[8];//색상
	Translate_pos pos[8];
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;
	BoOL b;
	BOOL any;

};

struct point_3 {
	float x = 0;
	float y = 0;
	float z = 60;

	BOOL xBool;
	BOOL yBool;
};

static point_3 spherePos;
static point_3 spherePos2;
static point_3 pingpong[10];
static point_3 box[3];

int change_count;
int next_rot;
int st_help;
BOOL Save = false;
BOOL ani = FALSE;
BOOL Look = FALSE;

typedef struct Three {
	float size;
	float x;
	float y;
};

static Three sq[3];
//카메라-----------------
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;

static int __x;
static int __y;
static int __z;

static Cam camera;

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

struct Sh {
	double x = 0;
	double y = 0;
	double z = 0;

	double moveX = 0;
	double moveY = 0;
	double moveZ = 0;

	int count = 0;
};

static Sh rect[500];

GLdouble result[16] = { 0 };
GLdouble Identity[16] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

GLdouble result2[16] = { 0 };
GLdouble Identity2[16] = {
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

GLvoid MultiMatrix2(GLdouble y[16]) {
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

struct point {
	float x;
	float y;
};

QUAD index;
static BOOL change = FALSE;
static BOOL airDir = FALSE;
//은면제거
BOOL depth;
int depth_count;
//컬링
BOOL culling;
int culling_count;
//쉐이딩
BOOL shade;
int shade_count;
static point PlanePosition[360];
void SetupRC()
{
	for (int i = 0; i < 10; ++i) {
		pingpong[i].z = rand() % 45;
		pingpong[i].x = rand() % 45;
		pingpong[i].y = rand() % 45;
		if (rand() % 2 == 0) {
			pingpong[i].xBool = FALSE;
		}
		else
			pingpong[i].xBool = TRUE;

		if (rand() % 2 == 0)
			pingpong[i].yBool = FALSE;
		else
			pingpong[i].yBool = TRUE;
	}

	for (int i = 0; i < 3; ++i) {
		box[i].x = 45;
		box[i].y = -45;
		box[i].z = (2 - i) * (-10);
	}
	//초기화
	camera.moveEye(0, 0, -150);
	camera.moveEye(0, 50, 0);
	camera.rotateEye(0, 20, 0);
	spherePos.y = 0;
	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
}
void main(int argc, char *argv[]) {
	//초기화

	EYE.x = 0, EYE.y = 0, EYE.z = 300;//EYE백터 초기화
	AT.x = 0, AT.y = 0, AT.z = 0;//EYE백터 초기화
	UP.x = 0, UP.y = 1, UP.z = 0;//EYE백터 초기화

	//-------------
	//인덱스 버퍼 저장
	index.pos[0].x = -1;
	index.pos[0].y = -1;
	index.pos[0].z = 1;

	index.cl[0].R = 1;
	index.cl[0].G = 0;
	index.cl[0].B = 0;

	index.pos[1].x = 1;
	index.pos[1].y = -1;
	index.pos[1].z = 1;

	index.cl[1].R = 1;
	index.cl[1].G = 0;
	index.cl[1].B = 1;

	index.pos[2].x = 1;
	index.pos[2].y = -1;
	index.pos[2].z = -1;

	index.cl[2].R = 0;
	index.cl[2].G = 0;
	index.cl[2].B = 1;

	index.pos[3].x = -1;
	index.pos[3].y = -1;
	index.pos[3].z = -1;

	index.cl[3].R = 0;
	index.cl[3].G = 0;
	index.cl[3].B = 0;

	index.pos[4].x = -1;
	index.pos[4].y = 1;
	index.pos[4].z = 1;

	index.cl[4].R = 1;
	index.cl[4].G = 1;
	index.cl[4].B = 0;

	index.pos[5].x = 1;
	index.pos[5].y = 1;
	index.pos[5].z = 1;

	index.cl[5].R = 1;
	index.cl[5].G = 1;
	index.cl[5].B = 1;

	index.pos[6].x = 1;
	index.pos[6].y = 1;
	index.pos[6].z = -1;

	index.cl[6].R = 0;
	index.cl[6].G = 1;
	index.cl[6].B = 1;

	index.pos[7].x = -1;
	index.pos[7].y = 1;
	index.pos[7].z = -1;

	index.cl[7].R = 0;
	index.cl[7].G = 1;
	index.cl[7].B = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
	glutCreateWindow("ex16");
	// - 랜덤으로 시작 도형 설정하기
	//도형 그리기
	SetupRC();
	glutMotionFunc(Motion);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timerfunction, 1);
	srand(time(NULL));
	glutMainLoop();
}

static int openUP = 0;
static int openFront = 0;

static int srad = 10;

GLvoid DrawBall()
{
	for (int i = 0; i < 10; ++i) {
		glPushMatrix();
		glTranslatef(pingpong[i].x, pingpong[i].y, pingpong[i].z);
		glutSolidSphere(4, 4, 4);
		glPopMatrix();
	}

}


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
static int rad;
static int TreeSize = 30;
static int airRad = 0;
static int nowPosition = 0;
GLvoid DrawAirplane(GLvoid)
{
	glPushMatrix();
	{
		glTranslatef(PlanePosition[nowPosition].x, 0, PlanePosition[nowPosition].y);
		glTranslatef(-10, 0, 0);
		glRotatef(-nowPosition, 0, 1, 0);
		glTranslatef(10, 0, 0);

		glPushMatrix();
		if (airDir == FALSE)
			glRotatef(90, 0, 1, 0);
		else if (airDir == TRUE)
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
	glFrontFace(GL_CW);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//은면 여부
	if (depth) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	//컬링 여부
	if (culling) {
		//glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
	}
	else {
		//glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);
	}



	glPushMatrix();//-----------------------------------
	{
		camera.drawCamera();
		glPushMatrix();

		glPushMatrix();
		{
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
		}
		glPopMatrix();

		glPushMatrix();//-------------그리기 입력--------------------------
		{
			glTranslated(0, 100, 0);
			glPushMatrix(); {
				glScaled(240, 100, 240);

				if (shade) {
					glShadeModel(GL_SMOOTH);
				}
				else {
					glShadeModel(GL_FLAT);
				}


				glBegin(GL_POLYGON); {//아래


					glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
					glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

					glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
					glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

				}
				glEnd();

				glBegin(GL_POLYGON); {//왼
					glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
					glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

					glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
					glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

				}
				glEnd();

				glBegin(GL_POLYGON); {//뒷면

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

					glColor3f(index.cl[3].R, index.cl[3].G, index.cl[3].B);
					glVertex3f(index.pos[3].x, index.pos[3].y, index.pos[3].z);//Yellow

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow


				}
				glEnd();

				glBegin(GL_POLYGON); {//오른쪽면
					glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
					glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

					glColor3f(index.cl[2].R, index.cl[2].G, index.cl[2].B);
					glVertex3f(index.pos[2].x, index.pos[2].y, index.pos[2].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow               

					glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
					glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

				}
				glEnd();

				glPushMatrix();
				//glScalef(70, 70, 70);
				glTranslatef(0, index.pos[7].y, index.pos[7].z);
				glRotatef(-openUP, 1, 0, 0);
				glTranslatef(0, -index.pos[7].y, -index.pos[7].z);
				glBegin(GL_POLYGON); {//뚜껑

					glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
					glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

					glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
					glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow

					glColor3f(index.cl[6].R, index.cl[6].G, index.cl[6].B);
					glVertex3f(index.pos[6].x, index.pos[6].y, index.pos[6].z);//Yellow            

					glColor3f(index.cl[7].R, index.cl[7].G, index.cl[7].B);
					glVertex3f(index.pos[7].x, index.pos[7].y, index.pos[7].z);//Yellow

				}
				glEnd();
				glPopMatrix();

				glPushMatrix();

				glTranslatef(0, index.pos[0].y, index.pos[0].z);
				glRotatef(openFront, 1, 0, 0);
				glTranslatef(0, -index.pos[0].y, -index.pos[0].z);

				glBegin(GL_POLYGON); {//앞면
					glColor3f(index.cl[0].R, index.cl[0].G, index.cl[0].B);
					glVertex3f(index.pos[0].x, index.pos[0].y, index.pos[0].z);//Yellow

					glColor3f(index.cl[1].R, index.cl[1].G, index.cl[1].B);
					glVertex3f(index.pos[1].x, index.pos[1].y, index.pos[1].z);//Yellow

					glColor3f(index.cl[5].R, index.cl[5].G, index.cl[5].B);
					glVertex3f(index.pos[5].x, index.pos[5].y, index.pos[5].z);//Yellow


					glColor3f(index.cl[4].R, index.cl[4].G, index.cl[4].B);
					glVertex3f(index.pos[4].x, index.pos[4].y, index.pos[4].z);//Yellow

				}
				glEnd();
				glPopMatrix();
			}
			glPopMatrix();//상자 그리기 끝
			glColor3f(0.0f, 0.0f, 0.0f);
			glPopMatrix();
		}
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

static int startX, startY;
static int endX, endY;
static int dir = 0;
static BOOL left = FALSE;

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		startX = x;
		startY = y;
		left = TRUE;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left = FALSE;
	}
	glutPostRedisplay();

}

void Motion(int x, int y)
{

}



static BOOL openU = FALSE;
static BOOL openF = FALSE;


BOOL legBool = FALSE;
BOOL TreeBool = FALSE;
BOOL handBool = FALSE;

BOOL xBool = FALSE;
BOOL yBool = FALSE;
BOOL zBool = FALSE;

void Timerfunction(int value) {

	glutPostRedisplay(); //타이머에 넣는다.4
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
	if (openU == FALSE) {
		if (openUP > 0)
			openUP -= 1;
		else {
				srad -= 2;
				MultiMatrix2(rotateX_P3);
		}
	}
	else if (openU == TRUE) {
		if (openUP < 90)
			openUP += 1;
		else {
			srad += 2;
			MultiMatrix2(rotateX_M3);
		}
	}

	if (openF == FALSE) {
		if (openFront > 0)
			openFront -= 1;
	}

	else if (openF == TRUE) {
		if (openFront < 90)
			openFront += 1;
	}


	glutTimerFunc(10, Timerfunction, 1); //타이머 다시 출력
}

int ttt;
int ani_count;
void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//---------카메라
		//rotate
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

	case '3':
		if (openU == FALSE)
			openU = TRUE;
		else
			openU = FALSE;
		break;

	case '4':
		if (openF == FALSE)
			openF = TRUE;
		else
			openF = FALSE;
		break;

		// z는 그대로 camera.z 

		//-----------카메라 끝 --------

		//------------투영 끝

	case '1':
		BallDir = 1;
		break;

	case '2':
		BallDir = 2;
		break;


	case '6'://은면제거
		depth_count++;
		if (depth_count % 2 == 1) {
			depth = true;
			printf("은면 On \n");
		}
		else {
			printf("은면 Off \n");
			depth = false;
		}

		break;
	case '7'://컬링
		culling_count++;
		if (culling_count % 2 == 1) {
			culling = true;
			printf("컬링 On \n");
		}
		else {
			printf("컬링 Off \n");
			culling = false;
		}

		break;
	case '8'://쉐이딩
		shade_count++;
		if (shade_count % 2 == 1) {
			shade = true;
			printf("쉐이딩 On \n");
		}
		else {
			printf("쉐이딩 Off \n");
			shade = false;
		}

		break;

	default:
		;
		break;
	}
	glutPostRedisplay();

}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, WideSize / HighSize, 1, Z_Size); //윈도우를 초기화 하는 함수입니다!
	//glTranslatef(0, 0, -300);


	if (!ani) {
		//glRotatef(30, 0, 1, 0);
		gluPerspective(60.0f, w / h, 1.0, 1000.0);

		glTranslated(0.0, 0.0, -300.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else {
		glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
		glTranslated(WideSize / 2, HighSize / 2, 0);
		//glRotatef(-60, 1, 0, 0);

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}