#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>

#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DXToRadian(degree)((degree) * (D3DX_PI / 180.f))
#define D3DXToDegree(radian)((radian) * (180.f / D3DX_PI))



// ���� ���ڸ� �׾����� �� �β��� ���� ���� 0���̰� �ð�������� 1��, 2��... �ѹ��� ���� �β��� ���������� 360���̴�.

// ���� = ���� ��, Ȯ�� �� ��

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

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 1;

static int select = -100;
static int right = 0;
static BOOL change = FALSE;
static int changeCount = 0;
static BOOL chOn = FALSE;


GLfloat ctrlpoints[80][3] = { 0, 0, 0 };
GLfloat Wall[100][3] = { 0,0,0 };
GLfloat ctrlpoints2[80][3] = { 0,0,0 };
GLfloat TreePos[10][3] = { 0, 0, 0 };

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


static Cam camera;
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
	}
	camera.rotateEye(0, 40, 0);
	topView.rotateEye(0, 90, 0);
}

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

GLvoid DrawTree(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.9f, 0.4f, 0.4f);
	glutSolidCylinder(10, 30, 20, 20);
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
			float y = ((pow(t, 3) * -1.f + 2.f *pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;
			
			glVertex3f(x, 100, y);
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
			float y = ((pow(t, 3) * -1.f + 2.f * pow(t, 2) - t) * p1[2] + (3.f * pow(t, 3) - 5.f * pow(t, 2) + 2.f) * p2[2] + (-3.f * pow(t, 3) + 4.f * pow(t, 2) + t) * p3[2] + (pow(t, 3) - pow(t, 2)) * p4[2]) / 2.f;

			glVertex3f(x * 0.7, 100, y * 0.7);
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
	glutSolidCylinder(5, 50, 10, 10);
	glPopMatrix();
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();


	if (cameraNum == 0)
		camera.drawCamera();
	else
		topView.drawCamera();
	
	for (int i = 0; i < count; ++i)
		DrawWall(Wall[i][0], Wall[i][1], Wall[i][2]);

	drawRail();

	drawPlan();

	for (int i = 0; i < 10; ++i) {
		DrawTree(TreePos[i][0], TreePos[i][1], TreePos[i][2]);
	}



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

void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1, TimerFunction, 1);
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

static float rad;

void Mouse(int button, int state, int x, int y)
{

	if (cameraNum == 1) {

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			ctrlpoints[count][0] = x - 400;
			ctrlpoints[count][1] = 100;
			ctrlpoints[count][2] = -(300 - 1 - y);
			Wall[count][0] = x - 400;
			Wall[count][1] = 50;
			Wall[count][2] = -(300 - 1 - y);
			count ++;
		}

	}
	glutPostRedisplay();

}






void SpecialKeys(int key, int x, int y) {

	glutPostRedisplay();
}