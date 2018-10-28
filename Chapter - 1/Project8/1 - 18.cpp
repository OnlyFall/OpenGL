#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
static int mode = 1;

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

#define PT 20//���� ���� ����
#define PI 3.141592 //����
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

Shape camera;
Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;
//
//struct Camera {
//	int xRad = -30;
//	int yRad;
//	int zRad;
//
//	float x;
//	float y;
//	float z;
//
//	BOOL xBOOL = FALSE;
//	int xDir;
//	BOOL yBOOL = FALSE;
//	int yDir;
//	BOOL zBOOL = FALSE;
//	int zDir;
//};
//
////static Camera camera;
//
//
//static Translate_pos camera;

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

static float R;
static float G;
static float B;

static int count = 0;
static int temp = 0;
static BOOL change = FALSE;


void SetupRC()
{
	//EYE.x = 0;
	//EYE.y = 30;
	//EYE.z = 0;
	camera.move.x = 0;
	camera.move.y = 0;
	camera.move.z = 0;
	// �ʿ��� ������, ��ǥ�� ���� �ʱ�ȭ // ��� ���� �ʱ�ȭ
}

const void camera_custom
(double pos_x, double pos_y, double pos_z, //��ġ
	double degree, const double rot_x, const double rot_y, const double rot_z, //ȸ��
	const double move_x, const double move_y, const double move_z //������
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

void main(int agrc, char *argv[]) { // ������ �ʱ�ȭ �� ���� 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// ���� ���۸��� 3�������� �׸���. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// ���� ���� �ʱ�ȭ �Լ� 
	SetupRC();
	// �ʿ��� �ݹ� �Լ� ����
	glutDisplayFunc(drawScene); // ��� �ݹ� �Լ� 
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ� �Լ�
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ� �Լ�
	glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸� �ݹ� �Լ�
	glutMainLoop(); // �̺�Ʈ ���� �����ϱ�
}
static int rad;
static int rad2;
static int rad3;
static int rad4;

static int xB, yB, zB;

static int shape = 0;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);




	glPushMatrix();
	glTranslatef(camera.move.x, camera.move.y , camera.move.z);
	gluLookAt(
		EYE.x, EYE.y, EYE.z,  //��5 eye
		AT.x, AT.y, AT.z, //���� center
		0, 1, 0); //���ʹ�

	camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);

	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glScalef(1, 0.01, 1);
		glutSolidCube(400);
		glPopMatrix();
	}

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

		glTranslatef(25,25, 10);
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

		glTranslatef(0, 15, 25);
	//	glRotatef(30, 1, 0, 0);
		glScalef(0.1, 0.1, 1);
		glutSolidCube(50);
		glPopMatrix();
	}
	



	if (shape == 1) {

		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidCube(50);
		else
			glutWireCube(50);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireCube(50);
		else
			glutSolidCube(50);
		glPopMatrix();
	}

	else if (shape == 2) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidTeapot(50);
		else
			glutWireTeapot(50);
		glPopMatrix();
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireTeapot(50);
		else
			glutSolidTeapot(50);
		glPopMatrix();
	}

	else if (shape == 3) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidSphere(50, 8, 8);
		else
			glutWireSphere(50, 8, 8);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireSphere(50, 8, 8);
		else
			glutSolidSphere(50, 8, 8);
		glPopMatrix();
	}

	else if (shape == 4) {
		glPushMatrix();
		glColor4f((float)R / 255, (float)G / 255, (float)B / 255, 1.0f);
		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(-100, 70, 0);
		glRotatef(rad2, 0, 1, 0);
		if (change == FALSE)
			glutSolidCone(50, 50, 8, 8);
		else
			glutWireCone(50, 50, 8, 8);
		glPopMatrix();

		glPushMatrix();
		glRotatef(xB, 1, 0, 0);
		glRotatef(yB, 0, 1, 0);
		glRotatef(zB, 0, 0, 1);

	//	glRotatef(30, 1, 0, 0);
		glRotatef(rad, 0, 1, 0);
		glTranslatef(100, 70, 0);
		glRotatef(rad3, 0, 1, 0);
		if (change == FALSE)
			glutWireCone(50, 50, 8, 8);
		else
			glutSolidCone(50, 50, 8, 8);

		glPopMatrix();
	}

	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
	//	glFlush(); // ȭ�鿡 ����ϱ� 
}

static float zZoom = 45;
GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(0, 0, w, h);  // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
	if (mode == 1) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//-- ������ ���� ���� �Ǵ� ���� ���� �� �� ���� �����Ѵ�. // 1. Ŭ���� ���� ����: ���� ������ ���
		gluPerspective(zZoom, 1.0, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -500.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
	}
	else if(mode == 2)
		glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

static int dir = 1;
static BOOL rad1Bool = FALSE;
static BOOL rad2Bool = FALSE;
static BOOL rad3Bool = FALSE;
static BOOL rad4Bool = FALSE;

static BOOL xBool = FALSE;
static BOOL yBool = FALSE;
static BOOL zBool = FALSE;

static BOOL rBool = FALSE;
static BOOL gBool = FALSE;
static BOOL bBool = FALSE;

void TimerFunction(int value)
{
	glutPostRedisplay();

	if (rad1Bool == TRUE)
		rad += 10;

	if (rad2Bool == TRUE)
		rad2 += 10;

	if (rad3Bool == TRUE)
		rad3 += 10;

	//if (rad4Bool == TRUE)
	//	rad4 += 10;

	//if (xBool == TRUE)
	//	xB = (xB + 10) % 360;

	//if (yBool == TRUE)
	//	yB = (yB + 10) % 360;

	//if (zBool == TRUE)
	//	zB = (zB + 10) % 360;

	if (rBool == false) {
		R += 3;
		if (R >= 252)
			rBool = TRUE;
	}
	else if (rBool == TRUE) {
		R -= 3;
		if (R <= 3)
			rBool = FALSE;
	}

	if (gBool == false) {
		G += 5;
		if (G >= 250)
			gBool = TRUE;
	}
	else if (gBool == TRUE) {
		G -= 5;
		if (G <= 5)
			gBool = FALSE;
	}

	if (bBool == false) {
		B += 1;
		if (B >= 254)
			bBool = TRUE;
	}
	else if (bBool == TRUE) {
		B -= 1;
		if (B <= 1)
			bBool = FALSE;
	}
	glutTimerFunc(100, TimerFunction, 1);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'L':
		if (rad2Bool == FALSE)
			rad2Bool = TRUE;
		else if (rad2Bool == TRUE)
			rad2Bool = FALSE;
		break;

	case 'R':
		if (rad3Bool == FALSE)
			rad3Bool = TRUE;
		else if (rad3Bool == TRUE)
			rad3Bool = FALSE;

		break;

	case 'O':
		if (rad1Bool == FALSE)
			rad1Bool = TRUE;
		else if (rad1Bool == TRUE)
			rad1Bool = FALSE;

		break;

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
		camera.rot.x -= 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;
	case 'X':
		camera.rot.x += 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;

	case 'y':
		camera.rot.y -= 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;
	case 'Y':
		camera.rot.y += 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;

	case 'z':
		camera.rot.z -= 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;
	case 'Z':
		camera.rot.z += 0.1;
		camera_custom(0, 0, 0, camera.rot.degree, camera.rot.x, camera.rot.y, camera.rot.z, camera.move.x, camera.move.y, camera.move.z);
		break;

		//move
	case 'w':
		camera.move.y += 1;
		break;
	case 'a':
		camera.move.x -= 1;
		break;

	case 's':
		camera.move.y -= 1;
		break;

	case 'd':
		camera.move.x += 1;
		break;
	case '+':
		camera.move.z += 1;
		break;
	case '-':
		camera.move.z -= 1;
		break;
	case 'i':
		camera.rot.degree = 0;
		camera.move.x = 0;
		camera.move.y = 0;
		camera.move.z = 0;
		camera.rot.x = 0;
		camera.rot.y = 0;
		camera.rot.z = 0;

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
		shape = 1;
		break;

	case '2':
		shape = 2;
		break;

	case '3':
		shape = 3;
		break;

	case 'C':
		if (change == FALSE)
			change = TRUE;
		else
			change = FALSE;
		break;

	case '4':
		shape = 4;
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

//#include <GL/freeglut.h> 
//#include <random>
//#include <time.h>
//#include <math.h>
//GLvoid Reshape(int w, int h);
//
////�ػ� ����
//#define WideSize 800
//#define HighSize 600
//#define Z_Size 400
//
////�׸��� ����
//GLvoid drawScene(GLvoid);
//BOOL draw_Act_On;
//
////Ÿ�̸� ����
//void Timerfunction(int value);
//BOOL Time_Act_On; //Ÿ�̸� Ȱ��ȭ ����
//int Time_count; //Ÿ�̸� ī��Ʈ
//
//				//���콺 ����
//void Mouse(int button, int state, int x, int y);
//POINT ms_click; //���콺 ��ǥ
//BOOL Mouse_Act_On; //���콺 Ȱ��ȭ ����
//
//				   //Ű���� ����
//void Keyboard(unsigned char key, int x, int y);
//BOOL Keyboard_Act_On; //Ű���� Ȱ��ȭ ����
//
//					  //�� ����
//typedef struct Color
//{
//	int R;
//	int G;
//	int B;
//};
//typedef struct Translate_pos {
//	GLdouble x;
//	GLdouble y;
//	GLdouble z;
//	float degree;
//};
//
//#define PT 20//���� ���� ����
//#define PI 3.141592 //����
//typedef struct Shape
//{
//	Color cl;//����
//	Translate_pos pos;
//	Translate_pos move;
//	Translate_pos scale;
//	Translate_pos rot;
//
//	int size;
//	int select;
//	int height;
//	int slice;
//	int stacks;
//
//	BOOL any;
//
//};
//
//static float see = 0;
//int change_count;
//int select_count;
//int st_help;
//BOOL Save = false;
//BOOL ani = FALSE;
//BOOL Look = FALSE;
//
//Shape camera;
//Shape tra;
//int rot_count;
//int rot_command;
//
//Shape shape[2];
//Shape view;
//void SetupRC()
//{
//	//�ʱ�ȭ
//	glTranslated(WideSize / 2, HighSize / 2, Z_Size / 2);
//}
//void main(int argc, char *argv[]) {
//	//�ʱ�ȭ
//
//	camera.rot.x = 0;
//	camera.rot.y = 1;
//	camera.rot.z = 1;
//
//	shape[0].pos.x = 100;
//	shape[1].pos.x = 100;
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
//	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
//	glutCreateWindow("ex16");
//	// - �������� ���� ���� �����ϱ�
//	//���� �׸���
//	//SetupRC();
//
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(Reshape);
//	glutMouseFunc(Mouse);
//	glutKeyboardFunc(Keyboard);
//	glutTimerFunc(100, Timerfunction, 1);
//	srand(time(NULL));
//	glutMainLoop();
//}
//
//GLvoid drawScene(GLvoid)
//{
//	glEnable(GL_DEPTH_TEST);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	GLdouble x_rotate[16] = {
//		1, 0, 0, 0,
//		0, cos(camera.rot.degree), -sin(camera.rot.degree), 0,
//		0, sin(camera.rot.degree), cos(camera.rot.degree), 0,
//		0,0,0,1 };
//
//	GLdouble y_rotate[16] = {
//	};
//
//	glLoadMatrixd(x_rotate);
//
//	glPushMatrix(); {
//
//			
//		glLineWidth(2);
//		glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
//		glMatrixMode(GL_MODELVIEW);
//		//��ǥ�� �׸���
//		glPushMatrix(); {
//
//			glTranslated(0, -15, 0);
//			glScalef(1, 0.001, 1);
//			glutSolidCube(200);//���� �����
//			glPopMatrix();
//		}
//			
//		//��� ���� �׸���
//		glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
//
//		for (int i = 0; i < 3; i++) {
//			glPushMatrix();
//			if (i == 0) {
//				glColor3f((float)255 / 255, (float)0 / 255, (float)0 / 255);
//				glRotated(90, 1, 0, 0);
//			}
//			else if (i == 1) {
//				glColor3f((float)0 / 255, (float)255 / 255, (float)0 / 255);
//				glRotated(90, 0, 1, 0);
//			}
//			else if (i == 2) {
//				glColor3f((float)0 / 255, (float)0 / 255, (float)255 / 255);
//				glRotated(90, 0, 0, 1);
//			}
//			glScalef(1, 0.1, 0.1);
//			glutSolidCube(40);
//			glPopMatrix();
//		}//��ǥ�� �׸���
//
//		 //glRotatef(Time_count, 0, 1, 0);
//	}
//	glPopMatrix();
//
//	glutSwapBuffers();
//}
//
//void Mouse(int button, int state, int x, int y) {
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//
//	}
//	glutPostRedisplay();
//
//}
//void Timerfunction(int value) {
//
//	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
//	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���
//
//}
//void Keyboard(unsigned char key, int x, int y) {
//	switch (key)
//	{
//
//		//------------------ī�޶�------------------------
//	case 'x':
//		camera.rot.degree -= 0.1;
//		camera.rot.z = cos((3.141592 / 180.f) * camera.rot.degree) * 100;
//		camera.rot.y = sin((3.141592 / 180.f) * camera.rot.degree) * 100;
//		break;
//
//	case 'X':
//		camera.rot.degree += 1;
//		camera.rot.z = cos((3.141592 / 180.f) * camera.rot.degree) * 100;
//		camera.rot.y = sin((3.141592 / 180.f) * camera.rot.degree) * 100;
//		break;
//
//	case 'y':
//		camera.rot.degree -= 1;
//		camera.rot.x = cos(PI *camera.rot.degree * 10 / 90);
//		camera.rot.z = sin(PI * camera.rot.degree * 10 / 90);
//		break;
//
//	case 'Y':
//		camera.rot.degree += 1;
//		camera.rot.x = cos(PI *camera.rot.degree * 10 / 90);
//		camera.rot.z = sin(PI * camera.rot.degree * 10 / 90);
//		break;
//
//	case 'z':
//		camera.rot.degree -= 1;
//		camera.rot.x = cos(PI *camera.rot.degree * 10 / 90);
//		camera.rot.y = sin(PI * camera.rot.degree * 10 / 90);
//		break;
//
//	case 'Z':
//		camera.rot.degree += 1;
//		camera.rot.x = cos(PI *camera.rot.degree * 10 / 90);
//		camera.rot.y = sin(PI * camera.rot.degree * 10 / 90);
//		break;
//
//	case 'w':
//		camera.rot.x += 1;
//		break;
//
//	case 's':
//		camera.rot.x -= 1;
//		break;
//
//	case 'd':
//		camera.rot.y += 1;
//		break;
//
//	case 'a':
//		camera.rot.y -= 1;
//		break;
//
//	case '+':
//		camera.rot.z += 1;
//		break;
//
//	case '-':
//		camera.rot.degree -= 1;
//		break;
//
//	case 'i':
//		camera.rot.x = 0;
//		camera.rot.y = 1;
//		camera.rot.z = 1;
//		break;
//		//-----------ī�޶� �� --------
//	default:
//		;
//		break;
//	}
//	glutPostRedisplay();
//
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45, WideSize / HighSize, 1, Z_Size); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
//	glTranslatef(0, 0, -300);
//	glMatrixMode(GL_MODELVIEW);
//
//}