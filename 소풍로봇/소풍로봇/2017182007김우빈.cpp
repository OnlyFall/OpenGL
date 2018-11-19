#include <gl/freeglut.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>
#include "weather.h"
#include "robot.h"

#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DXToRadian(degree)((degree) * (D3DX_PI / 180.f))
#define D3DXToDegree(radian)((radian) * (180.f / D3DX_PI))

static float GetDirectionAngle(float x1, float y1, float x2, float y2)
{
	x2 -= x1;
	y2 -= y1;
	//float Angle = D3DXToDegree( atan2(y2, x2) );

	float Angle = atan2(y2, x2) * 57.3f;

	//if (0 > Angle)
	//	Angle += 360;

	return Angle;
}

double GetDegree(float x1, float y1, float x2, float y2)
{
	return (atan2(y2 - y1, x2 - x1) * 180 / 3.1415926535);
}

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 1;

static int select = -100;
static BOOL change = FALSE;
static int changeCount = 0;
static BOOL chOn = FALSE;

static float saveRad = 0;
static float saveRad2 = 0;
static float saveRad3 = 0;

GLfloat ctrlpoints[80][3] = { 0, 0, 0 };
GLfloat Wall[100][4] = { 0,0,0,0 };
GLfloat ctrlpoints2[80][3] = { 0,0,0 };
GLfloat TreePos[10][5] = { 0, 0, 0, 0, 0 };

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

	void CenterEye(double x, double y, double z)
	{
		m_Center.x = x;
		m_Center.y = y;
		m_Center.z = z;
	}

	void EyeEye(double x, double y, double z)
	{
		m_Eye.x = x;
		m_Eye.y = y;
		m_Eye.z = z;
	}
};


static Cam camera;
static Cam FrontView;
static Cam topView;
static Cam Roll;

static int cameraNum;
static int count = 0;
static int drawTrue = 0;


void Mouse(int button, int state, int x, int y);

void SetupRC()
{
	for (int i = 0; i < 10; ++i) {
		TreePos[i][0] = rand() % 400 - 200;
		TreePos[i][1] = 50;
		TreePos[i][2] = rand() % 400 - 200;
		TreePos[i][3] = rand() % 10 + 20;
		TreePos[i][4] = rand() % 2;
	}
	camera.rotateEye(0, 40, 0);
	topView.rotateEye(0, 90, 0);
	FrontView.moveEye(0, 0, 100);

	for (int i = 0; i < 2; ++i) {
		robot[i].x = rand() % 300 - 150;
		robot[i].z = rand() % 300 - 150;
		robot[i].seeDir = rand() % 4 + 1;
	}
	Roll.Init();
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
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수

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

GLvoid DrawTree(float x, float y, float z, float size)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.9f, 0.4f, 0.4f);
	glutSolidCylinder(10, 50, 20, 20);
	glPopMatrix();

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(size, size, size);
	glPopMatrix();
}

GLvoid drawRail(GLvoid)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	BOOL check = FALSE;
	//int i = 0;
	//int temp = count - 1;
	//check = FALSE;
	//i = 0;



	for (int i = 0; i < count; ++i) {
		float *p1 = ctrlpoints[(i + count - 3) % count];
		float *p2 = ctrlpoints[(i + count - 2) % count];
		float *p3 = ctrlpoints[(i + count - 1) % count];
		float *p4 = ctrlpoints[(i + count - 0) % count];
		

	
		glColor3f(1.0f, 0.0f, 0.0f);


		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 100; ++j)
		{
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f *pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

			glVertex3f(x, y, z);
		}
		glEnd();

		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(ctrlpoints[i][0], ctrlpoints[i][1], ctrlpoints[i][2]);
		glEnd();
	}


	/*for (int i = 0; i < count; ++i) {
		float *p1 = ctrlpoints[(i + count - 3) % count];
		float *p2 = ctrlpoints[(i + count - 2) % count];
		float *p3 = ctrlpoints[(i + count - 1) % count];
		float *p4 = ctrlpoints[(i + count - 0) % count];

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 100; ++j)
		{
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;


			
			float tmp_x = x;
			float tmp_z = z;
			float temp = tmp_x;
			tmp_x = -tmp_z;
			tmp_z = temp;

			float vectsize = sqrt((tmp_x * tmp_x) + (tmp_z * tmp_z));
			tmp_x = tmp_x / vectsize;
			tmp_z = tmp_z / vectsize;
			
			float angle = acos(tmp_x *1.f + tmp_z * 0.f);
			if(tmp_x *0.f - tmp_z *1.f <= 0.f)
				angle *= -1;
			glVertex3f(x + cos(angle) * 20, y, z - sin(angle) * 20);
		}
		glEnd();

		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(ctrlpoints[i][0], ctrlpoints[i][1], ctrlpoints[i][2]);
		glEnd();
	}*/

	for (int i = 0; i < count; ++i) {
		float *p1 = ctrlpoints[(i + count - 3) % count];
		float *p2 = ctrlpoints[(i + count - 2) % count];
		float *p3 = ctrlpoints[(i + count - 1) % count];
		float *p4 = ctrlpoints[(i + count - 0) % count];

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 100; ++j)
		{
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

			glVertex3f(x * 0.9, y + 5, z * 0.9);
		}
		glEnd();

		for (int j = 0; j <= 100; j += 20) {
			glBegin(GL_LINES);
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;
			
			glVertex3f(x, y, z);
			glVertex3f(x*1.1, y + 5, z*1.1);
			glEnd();
		}

		for (int j = 0; j <= 100; j += 20) {
			glBegin(GL_LINES);
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

			glVertex3f(x, y, z);
			glVertex3f(x*0.9, y + 5, z*0.9);
			glEnd();
		}

		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(ctrlpoints[i][0], ctrlpoints[i][1], ctrlpoints[i][2]);
		glEnd();
	}


	for (int i = 0; i < count; ++i) {
		float *p1 = ctrlpoints[(i + count - 3) % count];
		float *p2 = ctrlpoints[(i + count - 2) % count];
		float *p3 = ctrlpoints[(i + count - 1) % count];
		float *p4 = ctrlpoints[(i + count - 0) % count];

		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 100; ++j)
		{
			float t = j / 100.f;
			float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
			float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

			glVertex3f(x * 1.1, y + 5, z * 1.1);
		}
		glEnd();

		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(ctrlpoints[i][0], ctrlpoints[i][1], ctrlpoints[i][2]);
		glEnd();
	}

	
}

GLvoid DrawWall(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0.5f, 0.f, 1.0f);
	glRotatef(90, 1, 0, 0);
	glutWireCylinder(4, y, 10, 10);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidCylinder(3, y, 10, 10);
	glPopMatrix();


}


GLvoid DrawTunel()
{
	glColor3f(0.7f, 0.7f, 0.7f);
	GLfloat tunel[3][3][3];
	float *p1 = ctrlpoints[(1 + count - 3) % count];
	float *p2 = ctrlpoints[(1 + count - 2) % count];
	float *p3 = ctrlpoints[(1 + count - 1) % count];
	float *p4 = ctrlpoints[(1 + count - 0) % count];

	float t = 10 / 100.f;
	float t1 = 50 / 100.f;
	float t2 = 80 / 100.f;
	
	float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
	float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
	float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

	float x1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[0] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[0] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[0] + (pow(t1, 3) - pow(t1, 2)) * p4[0]) / 2.f;
	float y1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[1] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[1] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[1] + (pow(t1, 3) - pow(t1, 2)) * p4[1]) / 2.f;
	float z1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[2] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[2] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[2] + (pow(t1, 3) - pow(t1, 2)) * p4[2]) / 2.f;

	float x2 = ((pow(t2, 3) * -1.f + 2.f * pow(t2, 2) - t2) * p1[0] + (3.f * pow(t2, 3) - 5.f * pow(t2, 2) + 2.f) * p2[0] + (-3.f * pow(t2, 3) + 4.f * pow(t2, 2) + t2) * p3[0] + (pow(t2, 3) - pow(t2, 2)) * p4[0]) / 2.f;
	float y2 = ((pow(t2, 3) * -1.f + 2.f * pow(t2, 2) - t2) * p1[1] + (3.f * pow(t2, 3) - 5.f * pow(t2, 2) + 2.f) * p2[1] + (-3.f * pow(t2, 3) + 4.f * pow(t2, 2) + t2) * p3[1] + (pow(t2, 3) - pow(t2, 2)) * p4[1]) / 2.f;
	float z2 = ((pow(t2, 3) * -1.f + 2.f * pow(t2, 2) - t2) * p1[2] + (3.f * pow(t2, 3) - 5.f * pow(t2, 2) + 2.f) * p2[2] + (-3.f * pow(t2, 3) + 4.f * pow(t2, 2) + t2) * p3[2] + (pow(t2, 3) - pow(t2, 2)) * p4[2]) / 2.f;

	tunel[0][0][0] = x * 0.8;
	tunel[0][0][1] = y;
	tunel[0][0][2] = z * 0.8;

	tunel[1][0][0] = x;
	tunel[1][0][1] = y + 100;
	tunel[1][0][2] = z;

	tunel[2][0][0] = x * 1.2;
	tunel[2][0][1] = y;
	tunel[2][0][2] = z * 1.2;

	tunel[0][1][0] = x1 * 0.8;
	tunel[0][1][1] = y1;
	tunel[0][1][2] = z1 * 0.8;

	tunel[1][1][0] = x1;
	tunel[1][1][1] = y1 + 100;
	tunel[1][1][2] = z1;

	tunel[2][1][0] = x1 * 1.2;
	tunel[2][1][1] = y1;
	tunel[2][1][2] = z1 * 1.2;

	tunel[0][2][0] = x2 * 0.8;
	tunel[0][2][1] = y2;
	tunel[0][2][2] = z2 * 0.8;

	tunel[1][2][0] = x2;
	tunel[1][2][1] = y2 + 100;
	tunel[1][2][2] = z2;

	tunel[2][2][0] = x2 * 1.2;
	tunel[2][2][1] = y2;
	tunel[2][2][2] = z2 * 1.2;

	
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &tunel[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	// 그리드를 이용한 곡면 드로잉 
	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
	// 선을 이용하여 그리드 연결
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
	glDisable(GL_MAP2_VERTEX_3);
	

	glBegin(GL_LINES);
	glVertex3f(x * 0.8, y , z * 0.8);
	glVertex3f(x , y, z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x * 1.2, y, z * 1.2);
	glVertex3f(x, y, z);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x1 * 0.8, y1, z1 * 0.8);
	glVertex3f(x1, y1, z1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x1 * 1.2, y1, z1 * 1.2);
	glVertex3f(x1, y1, z1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x2 * 0.8, y2, z2 * 0.8);
	glVertex3f(x2, y2, z2);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2 * 1.2, y2, z2 * 1.2);
	glEnd();

}

GLvoid Snow()
{
	//selectWeather = 1;

	for (int i = 0; i < 100; ++i) {
		glPushMatrix();
		glTranslatef(snow[i].x, snow[i].y, snow[i].z);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(5, 5, 5);
		glPopMatrix();
	}
}

GLvoid Rainism()
{
	//selectWeather = 1;

	for (int i = 0; i < 100; ++i) {
		glPushMatrix();
		glTranslatef(rain[i].x, rain[i].y, rain[i].z);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.0f, 0.1f, 1.0f);
		glutSolidCylinder(2, 15, 15, 15);
		glPopMatrix();
	}
}


GLvoid DrawCar(int j, int c, int num)
{
	glPushMatrix();
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		float *p1 = ctrlpoints[(c + count - 3) % count];
		float *p2 = ctrlpoints[(c + count - 2) % count];
		float *p3 = ctrlpoints[(c + count - 1) % count];
		float *p4 = ctrlpoints[(c + count - 0) % count];

		float t = j / 100.f;
		float t1 = (j + 1) / 100.f;
		float x = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[0] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[0] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[0] + (pow(t, 3) - pow(t, 2)) * p4[0]) / 2.f;
		double y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
		float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

	
		float x1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[0] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[0] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[0] + (pow(t1, 3) - pow(t1, 2)) * p4[0]) / 2.f;
		double y1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[1] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[1] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[1] + (pow(t1, 3) - pow(t1, 2)) * p4[1]) / 2.f;
		float z1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[2] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[2] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[2] + (pow(t1, 3) - pow(t1, 2)) * p4[2]) / 2.f;
	
		
		int radi;

		if (j == 100) {
			int tmp = c + 1;
			float *s1 = ctrlpoints[(tmp + count - 3) % count];
			float *s2 = ctrlpoints[(tmp + count - 2) % count];
			float *s3 = ctrlpoints[(tmp + count - 1) % count];
			float *s4 = ctrlpoints[(tmp + count - 0) % count];

			float k = 1 / 100.f;
			
			float x2 = ((pow(k, 3) * -1.f + 2.f * pow(k, 2) - k) * s1[0] + (3.f * pow(k, 3) - 5.f * pow(k, 2) + 2.f) * s2[0] + (-3.f * pow(k, 3) + 4.f * pow(k, 2) + k) * s3[0] + (pow(k, 3) - pow(k, 2)) * s4[0]) / 2.f;
			double y2 = ((pow(k, 3) * -1.f + 2.f * pow(k, 2) - k) * s1[1] + (3.f * pow(k, 3) - 5.f * pow(k, 2) + 2.f) * s2[1] + (-3.f * pow(k, 3) + 4.f * pow(k, 2) + k) * s3[1] + (pow(k, 3) - pow(k, 2)) * s4[1]) / 2.f;
			float z2 = ((pow(k, 3) * -1.f + 2.f * pow(k, 2) - k) * s1[2] + (3.f * pow(k, 3) - 5.f * pow(k, 2) + 2.f) * s2[2] + (-3.f * pow(k, 3) + 4.f * pow(k, 2) + t) * s3[2] + (pow(k, 3) - pow(k, 2)) * s4[2]) / 2.f;

			radi = GetDegree(x, y, x2, y2);
		}
		else
			radi = GetDegree(x, y, x1, y1);
		float rad1 = GetDegree(x1, z1, x, z);


	
		glPushMatrix();
		Roll.Init();
		Roll.CenterEye(x1, y1 + 20, z1);
		Roll.EyeEye(x, y + 20, z);
		//Roll.rotateEye(0, rad1, 0);
		glTranslatef(x, y, z);

	//	glRotatef(rad, 0, 0, 1);
		glPushMatrix();
		
		//if(num == 1)
		//	glRotatef(rad1, 0, 1, 0);
		//else if (num == 2)
		//	glRotatef(rad1, 0, 1, 0);
		//else if (num == 3)
		glRotatef(90 - rad1, 0, 1, 0);
	
		
		glPushMatrix();
		glRotatef(radi, 1, 0, 0);
		glutSolidCube(20);
		glColor3f(1.0f, 0.0f, 1.0f);
		glutWireCube(21);
		glPopMatrix();

		glPopMatrix();

		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glVertex3f(x1,y1 ,z1);
		glEnd();
		glPopMatrix();

	}
	glPopMatrix();
}




GLvoid DrawRobot(int i)
{
	glPushMatrix();
	glTranslatef(robot[i].x, robot[i].y, robot[i].z);
	
	if(robot[i].seeDir == 1)
		glRotatef(90, 0, 1, 0);
	else if(robot[i].seeDir == 2)
		glRotatef(180, 0, 1, 0);
	else if(robot[i].seeDir == 3)
		glRotatef(270, 0, 1, 0);
	else if (robot[i].seeDir == 4)
		glRotatef(0, 0, 1, 0);
	//로봇 그리기 장소
	RobotHead();
	RobotMiddle();
	RobotLeg(robot[i].roboRad);
	RobotHand(robot[i].roboRad);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(robot[i].x, robot[i].y, robot[i].z);
	if(robot[i].seeDir == 1)
		glRotatef(90, 0, 1, 0);
	else if (robot[i].seeDir == 2)
		glRotatef(180, 0, 1, 0);
	else if (robot[i].seeDir == 3)
		glRotatef(270, 0, 1, 0);
	else if (robot[i].seeDir == 4)
		glRotatef(0, 0, 1, 0);
	glTranslatef(-robot[i].x, -robot[i].y, -robot[i].z);
	korea(i);
	glPopMatrix();
}



static int checkCar = 0;
static int nextStage = 0;

static int checkCar1 = 20;
static int nextStage1 = 0;

static int checkCar2 = 40;
static int nextStage2 = 0;
GLfloat fogColor[] = { 1.0f, 1.0f, 1.0f, 0.2f };
GLfloat am[] = { 0.0f, 300.0f, 0.0f, 1.0f};
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.25f, 0.25f, 0.25f, 0.2f };
GLfloat specu[] = { 1.0f, 1.0f, 1.0f, 0.1f };
GLfloat gray[] = { 0.25f, 0.25f, 0.25f, 0.1f };


static BOOL fogOn = TRUE;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	if (fogOn == TRUE) {
		glEnable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 1.0f);
		//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

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

		

		/*glEnable(GL_FOG);
		glFogf(GL_FOG_MODE, GL_LINEAR);*/
	}
	else {
		glDisable(GL_FOG);
		glDisable(GL_LIGHT0);
	}

	glPushMatrix();


	if (cameraNum == 0)
		camera.drawCamera();
	else if (cameraNum == 1)
		topView.drawCamera();
	else if (cameraNum == 2)
		FrontView.drawCamera();
	else if (cameraNum == 3)
		Roll.drawCamera();

	for (int i = 0; i < count; ++i)
		DrawWall(Wall[i][0], Wall[i][1], Wall[i][2]);

	drawRail();
	if (count > 4) {
		DrawCar(checkCar, nextStage,1);
		DrawCar(checkCar1, nextStage1,2);
		DrawCar(checkCar2, nextStage2,3);
	}
	drawPlan();

	for (int i = 0; i < 10; ++i) {
		DrawTree(TreePos[i][0], TreePos[i][1], TreePos[i][2], TreePos[i][3]);
	}

	DrawRobot(0);
	DrawRobot(1);

	if(count > 2)
		DrawTunel();

	if (selectWeather == 1)
		Rainism();
	else if (selectWeather == 2)
		Snow();


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
		if (cameraNum == 3)
			glTranslatef(0, 0, 500);
	}
	else if (mode == 2)
		glOrtho(-400.0, 400.0, -300.0, 300.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

static BOOL roboR = FALSE;
static BOOL roboR2 = FALSE;

void TimerFunction(int value)
{
	glutPostRedisplay();

	for (int i = 0; i < 10; ++i) {
		if (TreePos[i][4] == 0) {
			TreePos[i][3] += 0.1;
			if (TreePos[i][3] > 30)
				TreePos[i][4] = 1;
		}

		else if (TreePos[i][4] == 1) {
			TreePos[i][3] -= 0.1;
			if (TreePos[i][3] < 10)
				TreePos[i][4] = 0;
		}
	}
	if (count > 2) {
		checkCar++;
		if (checkCar == 101) {
			checkCar = 0;
			nextStage = (nextStage + 1) % count;
		}

		checkCar1++;
		if (checkCar1 == 101) {
			checkCar1 = 0;
			nextStage1 = (nextStage1 + 1) % count;
		}

		checkCar2++;
		if (checkCar2 == 101) {
			checkCar2 = 0;
			nextStage2 = (nextStage2 + 1) % count;
		}
	}

	if (selectWeather != 0) {

		for (int i = 0; i < 100; ++i) {
			rain[i].y -= 2;
			snow[i].y -= 2;
			if (rain[i].y < 0)
				rain[i].y = rand() % 300 + 100;
			if (snow[i].y < 0)
				snow[i].y = rand() % 300 + 100;
		}

	}

	for (int i = 0; i < 2; ++i) {
		if (robot[i].walk == FALSE) {
			robot[i].roboRad += 1;
			if (robot[i].roboRad >= 20)
				robot[i].walk = TRUE;
		}

		else if (robot[i].walk == TRUE) {
			robot[i].roboRad -= 1;
			if (robot[i].roboRad <= -20)
				robot[i].walk = FALSE;
		}


		if (crashCheck == FALSE) {
			if (robot[i].seeDir == 1) {

				for (int j = 0; j < 10; ++j) {
					if (robot[i].x + 0.2 >= TreePos[j][0] - 20 && robot[i].x + 0.2 <= TreePos[j][0] + 20 && robot[i].z >= TreePos[j][2] - 20 && robot[i].z <= TreePos[j][2] + 20)
						while (robot[i].seeDir == 1)
							robot[i].seeDir = rand() % 4 + 1;
				}
				if (robot[i].x >= 180)
					robot[i].seeDir = 2;
				robot[i].x += 0.2;


				if (robot[0].x - 20 >= robot[1].x + 20 && robot[0].x + 20 <= robot[1].x - 20 && robot[0].z - 20 >= robot[1].z + 20 && robot[0].z + 20 <= robot[1].z - 20)
					crashCheck = TRUE;

				if (robot[i].seeDir != 1)
					robot[i].x -= 0.4;

			}

			else if (robot[i].seeDir == 2) {

				for (int j = 0; j < 10; ++j) {
					if (robot[i].x - 0.2 >= TreePos[j][0] - 20 && robot[i].x - 0.2 <= TreePos[j][0] + 20 && robot[i].z >= TreePos[j][2] - 20 && robot[i].z <= TreePos[j][2] + 20)
						while (robot[i].seeDir == 2)
							robot[i].seeDir = rand() % 4 + 1;
				}
				if (robot[i].z <= -180)
					robot[i].seeDir = 3;
				robot[i].z -= 0.2;


				if (robot[0].x - 20 >= robot[1].x + 20 && robot[0].x + 20 <= robot[1].x - 20 && robot[0].z - 20 >= robot[1].z + 20 && robot[0].z + 20 <= robot[1].z - 20)
					crashCheck = TRUE;

				if (robot[i].seeDir != 2)
					robot[i].z += 0.4;
			}

			else if (robot[i].seeDir == 3) {


				for (int j = 0; j < 10; ++j) {
					if (robot[i].x >= TreePos[j][0] - 20 && robot[i].x <= TreePos[j][0] + 20 && robot[i].z - 0.2 >= TreePos[j][2] - 20 && robot[i].z - 0.2 <= TreePos[j][2] + 20)
						while (robot[i].seeDir == 3)
							robot[i].seeDir = rand() % 4 + 1;
				}

				if (robot[i].x <= -180)
					robot[i].seeDir = 4;

				robot[i].x -= 0.2;


				if (robot[0].x - 20 >= robot[1].x + 20 && robot[0].x + 20 <= robot[1].x - 20 && robot[0].z - 20 >= robot[1].z + 20 && robot[0].z + 20 <= robot[1].z - 20)
					crashCheck = TRUE;

				if (robot[i].seeDir != 3)
					robot[i].x += 0.4;
			}

			else if (robot[i].seeDir == 4) {


				for (int j = 0; j < 10; ++j) {
					if (robot[i].x >= TreePos[j][0] - 20 && robot[i].x <= TreePos[j][0] + 20 && robot[i].z + 0.2 >= TreePos[j][2] - 20 && robot[i].z + 0.2 <= TreePos[j][2] + 20)
						while (robot[i].seeDir == 4)
							robot[i].seeDir = rand() % 4 + 1;
				}


				if (robot[i].z >= 180)
					robot[i].seeDir = 1;
				robot[i].z += 0.2;

				if (robot[0].x - 20 >= robot[1].x + 20 && robot[0].x + 20 <= robot[1].x - 20 && robot[0].z - 20 >= robot[1].z + 20 && robot[0].z + 20 <= robot[1].z - 20)
					crashCheck = TRUE;

				if (robot[i].seeDir != 4)
					robot[i].z -= 0.4;
			}
		}
	}

	glutTimerFunc(10, TimerFunction, 1);
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

	case 't':
	case 'T':
		robot[1].seeDir = 2;
		break;

	case 'f':
	case 'F':
		robot[1].seeDir = 3;
		break;

	case 'g':
	case 'G':
		robot[1].seeDir = 4;
		break;

	case 'h':
	case 'H':
		robot[1].seeDir = 1;
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

	case '1':
		mode = 1;
		cameraNum = 0;
		Reshape(800, 600);
		break;

	case '2':
		mode = 2;
		cameraNum = 1;
		Reshape(800, 600);
		break;

	case '3':
		mode = 2;
		cameraNum = 2;
		Reshape(800, 600);
		break;
		
	case '4':
		mode = 1;
		cameraNum = 3;
		Reshape(800, 600);
		break;

	case '9':
		zZoom += 1;
		Reshape(800, 600);
		break;

	case '0':
		zZoom -= 1;
		Reshape(800, 600);
		break;

	case 'r':
	case 'R':

		if (selectWeather == 0)
			selectWeather = 1;
		else if (selectWeather == 1)
			selectWeather = 2;
		else if (selectWeather == 2)
			selectWeather = 0;
		
		if (selectWeather == 1) {
			for (int i = 0; i < 100; ++i) {
				rain[i].x = rand() % 400 - 200;
				rain[i].y = rand() % 200 + 100;
				rain[i].z = rand() % 400 - 200;
			}
		}
		else if (selectWeather == 2) {
			for (int i = 0; i < 100; ++i) {
				snow[i].x = rand() % 400 - 200;
				snow[i].y = rand() % 200 + 100;
				snow[i].z = rand() % 400 - 200;
			}
		}

		break;

	case 'o':
	case 'O':
		if (fogOn == FALSE)
			fogOn = TRUE;
		else
			fogOn = FALSE;
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}
static BOOL click = FALSE;
static float rad;
static BOOL right = FALSE;
void Mouse(int button, int state, int x, int y)
{
	bool buildOk = true;
	if (cameraNum == 1) {

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x - 400 > -200 && x - 400 < 200 && -(300 - 1 - y) > -200 && -(300 - 1 - y) < 200){
			for (int i = 0; i < 10; ++i) {
				if (TreePos[i][0] - 10 < x - 400 && TreePos[i][0] + 10 > x - 400 && TreePos[i][2] - 10 < -(300 - 1 - y) && TreePos[i][2] + 10 > -(300 - 1 - y))
					buildOk = false;
			}

			if (buildOk == true) {
				ctrlpoints[count][0] = x - 400;
				ctrlpoints[count][1] = 100;
				ctrlpoints[count][2] = -(300 - 1 - y);
				Wall[count][0] = x - 400;
				Wall[count][1] = 100;
				Wall[count][2] = -(300 - 1 - y);
				Wall[count][3] = 100;
				count++;
			}
		}
		buildOk = true;

		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			for (int i = 0; i < count; ++i) {
				if (ctrlpoints[i][0] - 10 < x - 400 && ctrlpoints[i][0] + 10 > x - 400 && ctrlpoints[i][2] - 10 < -(300 - 1 - y) && ctrlpoints[i][2] + 10 > -(300 - 1 - y)) {
					select = i;
					right = TRUE;
				}
			}
		}

		if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
			right = FALSE;
			select = -100;
		}
	}

	else if (cameraNum == 2) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			for (int i = 0; i < count; ++i) {
				if (ctrlpoints[i][0] - 4 < x - 400 && ctrlpoints[i][0] + 4 > x - 400  && ctrlpoints[i][1] > (300 - 1 - y) && ctrlpoints[i][1] - ctrlpoints[i][1] < (300 - 1 - y)) {
					select = i;
					click = TRUE;
				}
			}
		}

		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			click = FALSE;
			select = -100;
		}

	}
	glutPostRedisplay();

}



void Motion(int x, int y)
{
	if (click == TRUE) {
		ctrlpoints[select][1] = (300 - 1 - y);
		Wall[select][1] = (300 - 1 - y);
	}
	
	if (right == TRUE) {
		ctrlpoints[select][0] = x - 400;
		ctrlpoints[select][2] = -(300 - 1 - y);
		Wall[select][0] = x - 400;
		Wall[select][2] = -(300 - 1 - y);
	}
	glutPostRedisplay();  
}


void SpecialKeys(int key, int x, int y) {

	if (key == GLUT_KEY_DOWN)
		robot[0].seeDir = 4;

	else if (key == GLUT_KEY_UP)
		robot[0].seeDir = 2;
	else if (key == GLUT_KEY_RIGHT)
		robot[0].seeDir = 1;
	else if (key == GLUT_KEY_LEFT)
		robot[0].seeDir = 3;
	glutPostRedisplay();
}