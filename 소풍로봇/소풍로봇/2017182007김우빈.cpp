#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>

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
	return atan2(y2 - y1, x2 - x1) * 180 / 3.1415926535;
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
static int right = 0;
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
};


static Cam camera;
static Cam FrontView;
static Cam topView;
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

			glVertex3f(x * 0.9, y, z * 0.9);
		}
		glEnd();

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

			glVertex3f(x * 1.1, y, z * 1.1);
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
		float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[1] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[1] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[1] + (pow(t, 3) - pow(t, 2)) * p4[1]) / 2.f;
		float z = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

		float x1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[0] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[0] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[0] + (pow(t1, 3) - pow(t1, 2)) * p4[0]) / 2.f;
		float y1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[1] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[1] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[1] + (pow(t1, 3) - pow(t1, 2)) * p4[1]) / 2.f;
		float z1 = ((pow(t1, 3) * -1.f + 2.f * pow(t1, 2) - t1) * p1[2] + (3.f * pow(t1, 3) - 5.f * pow(t1, 2) + 2.f) * p2[2] + (-3.f * pow(t1, 3) + 4.f * pow(t1, 2) + t1) * p3[2] + (pow(t1, 3) - pow(t1, 2)) * p4[2]) / 2.f;

		glTranslatef(x, y, z);
		float rad = GetDirectionAngle(x, y, x1, y1);
		float rad1 = GetDegree(x, z, x1, z1);
		if (num == 1)
			saveRad += rad1;
		else if (num == 2)
			saveRad2 += rad1;
		else if (num == 3)
			saveRad3 += rad1;

		glRotatef(rad, 0, 0, 1);
		/*if(num == 1)
			glRotatef(rad1, 0, 1, 0);
		else if (num == 2)
			glRotatef(rad1, 0, 1, 0);
		else if (num == 3)
			glRotatef(rad1, 0, 1, 0);*/
		glutSolidCube(20);
		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glVertex3f(x1,y1 ,z1);
		glEnd();

	}
	glPopMatrix();
}


static int checkCar = 0;
static int nextStage = 0;

static int checkCar1 = 20;
static int nextStage1 = 0;

static int checkCar2 = 40;
static int nextStage2 = 0;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();


	if (cameraNum == 0)
		camera.drawCamera();
	else if (cameraNum == 1)
		topView.drawCamera();
	else if (cameraNum == 2)
		FrontView.drawCamera();

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
		if (checkCar == 100) {
			checkCar = 0;
			nextStage = (nextStage + 1) % count;
		}

		checkCar1++;
		if (checkCar1 == 100) {
			checkCar1 = 0;
			nextStage1 = (nextStage1 + 1) % count;
		}

		checkCar2++;
		if (checkCar2 == 100) {
			checkCar2 = 0;
			nextStage2 = (nextStage2 + 1) % count;
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
		Reshape(800, 600);
		cameraNum = 0;
		break;

	case '2':
		mode = 2;
		Reshape(800, 600);
		cameraNum = 1;
		break;

	case '3':
		mode = 2;
		Reshape(800, 600);
		cameraNum = 2;
		break;

	case '9':
		zZoom += 1;
		Reshape(800, 600);
		break;

	case '0':
		zZoom -= 1;
		Reshape(800, 600);
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}
static BOOL click = FALSE;
static float rad;
void Mouse(int button, int state, int x, int y)
{

	if (cameraNum == 1) {

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			ctrlpoints[count][0] = x - 400;
			ctrlpoints[count][1] = 100;
			ctrlpoints[count][2] = -(300 - 1 - y);
			Wall[count][0] = x - 400;
			Wall[count][1] = 100;
			Wall[count][2] = -(300 - 1 - y);
			Wall[count][3] = 100;
			count ++;
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
	glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y) {

	glutPostRedisplay();
}