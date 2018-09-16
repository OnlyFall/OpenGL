#include <gl/freeglut.h> 
#include <math.h>

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);

void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);

struct vertex {
	int x;
	int y;
};

struct circle {
	int x;
	int y;

	float r = 100;
	float g = 53;
	float b = 152;

	vertex circleVertex[36];
	int in;
	int count = 0;
	bool out = false;
};

static circle ci[100];
static int count = 0;
void main(int argc, char *argv[])
{
	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// ���÷��� ��� ���� 
	glutInitWindowPosition(0, 0);
	// �������� ��ġ���� 
	glutInitWindowSize(1920, 1080); // �������� ũ�� ����

	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ���� 
	glutReshapeFunc(Reshape); 
	glutMouseFunc(Mouse);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
} // ������ ��� �Լ� 


GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW
	for (int i = 0; i < count; ++i) {
		glBegin(GL_POINTS);
		if (ci[i].out == true)
			glColor4f((float)ci[i].r / 255, (float)ci[i].g / 255, (float)ci[i].b / 255, 0.0f);//YELLOW
		else
			glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW

		for (int j = 0; j < 36; ++j) {
			
			glVertex2f(ci[i].circleVertex[j].x, ci[i].circleVertex[j].y);
			
		}
		glEnd();
	}
	
	glFlush(); // ȭ�鿡 ����ϱ� 
}

GLvoid Reshape(int w, int h)    // �ٽ� �׸��� �Լ�
{
	glViewport(-1, -1, w, h);
	glOrtho(0, 1920, 1080, 0, 0, 1);
}

void Mouse(int button, int state, int x, int y) 
{
	int p;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ci[count].x = x;
		ci[count].y = y;
		ci[count].in = rand() % 500 + 10;
		p = rand() % 10 + 1;
		if (p == 1 || p == 5 || p == 7)
			ci[count].out = true;
		else
			ci[count].out = false;
		count++;
	}
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < 36; ++j) {
			ci[i].circleVertex[j].x = cos((3.141592 / 180.f) * 10 * (j + 1)) * ci[i].count + ci[i].x;
			ci[i].circleVertex[j].y = sin((3.141592 / 180.f) * 10 * (j + 1)) * ci[i].count + ci[i].y;
			if (ci[i].count == ci[i].in && ci[i].out == false)
				ci[i].count = 0;
		}
		ci[i].count++;
		if (ci[i].out == true) {
			ci[i].r += 10;
			ci[i].g += 5;
			ci[i].b += 7;
		}
	}
	glutTimerFunc(10, TimerFunction, 1);
}





// �׽�Ʈ���� ������ 7��
//#include <GL/freeglut.h> 
//#include <random>
//#include <time.h>
//#include <math.h>
//
//GLvoid Reshape(int w, int h);
//
////�ػ� ����
//#define WideSize 800
//#define HighSize 600
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
//					  //�÷�
//typedef struct Color
//{
//	int R;
//	int B;
//	int G;
//};
//
////���� ����
//#define cir_amount 20
//#define PI 3.14159265358
//
//typedef struct Circle {
//	POINT pt[36]; //360��
//	Color cl; //����
//	int rad;//������
//	int Ani_rad; //�ִϸ��̼� ����
//	POINT pos;//�߽� ��ǥ
//	int Max_rad; //�ƽ� ����
//				 //-------------------------------------
//	BOOL Ani;//������ ������ �ִϸ��̼� ����
//};
//
//int circle_count = 0;
//static Circle cir[cir_amount];
//
//void main(int argc, char *argv[]) {
//	//�ʱ�ȭ
//	for (int i = 0; i < cir_amount; i++) {
//		if (cir[i].Ani) {//�ִϸ��̼��� Ʈ����
//			cir[i].Max_rad = WideSize - cir[i].pos.x + 100;
//		}
//		else {
//			cir[i].Max_rad = 80; //���� ����
//		}
//		cir[i].cl.R = 255;
//		cir[i].cl.G = 255;
//		cir[i].cl.B = 255;
//	}
//	glutInit(&argc, argv);//������ ȭ�� �Ⱥ���
//
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
//	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
//	glutCreateWindow("ex7");
//	// - �������� ���� ���� �����ϱ�
//	//���� �׸���
//
//	glutDisplayFunc(drawScene);
//	glutMouseFunc(Mouse);
//	glutKeyboardFunc(Keyboard);
//	glutTimerFunc(100, Timerfunction, 1);
//	glutReshapeFunc(Reshape);
//
//	srand(time(NULL));
//	glutMainLoop();
//}
//
//GLvoid drawScene(GLvoid)
//{
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	//�׸��� ���   
//
//	for (int i = 0; i < cir_amount; i++) {
//		glPointSize(3);
//		glColor4f(
//			(float)cir[i].cl.R / 255,
//			(float)cir[i].cl.G / 255,
//			(float)cir[i].cl.B / 255,
//			1.0f
//		);
//		glBegin(GL_POINTS);
//		for (int j = 0; j < 36; j++) {
//			glVertex2f(
//				cir[i].pos.x + cir[i].pt[j].x,
//				cir[i].pos.y + cir[i].pt[j].y
//			);
//		}
//		glEnd();
//	}
//
//
//	glFlush();
//}
//
//void Mouse(int button, int state, int x, int y) {
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		cir[circle_count].pos.x = x;
//		cir[circle_count].pos.y = y;
//
//		cir[circle_count].cl.R = 100;
//		cir[circle_count].cl.G = 100;
//		cir[circle_count].cl.B = 200;
//
//
//		//circle ī��Ʈ �ʱ�ȭ
//		if (circle_count == 19) {
//			circle_count = 0;
//		}
//		else {
//			circle_count++;
//		}
//
//	}
//
//}
//
//void Timerfunction(int value) {
//
//	//Ÿ�̸� ���� �Է�
//	for (int i = 0; i < circle_count; i++) {
//		if (cir[i].rad < cir[i].Max_rad) {
//			cir[i].rad += 2; //������ ����
//		}
//		else {
//			//���� ������ �̻�� ���� ������� �Ѵ�.
//			cir[i].cl.R = 255;
//			cir[i].cl.G = 255;
//			cir[i].cl.B = 255;
//			cir[i].rad = 0;
//		}
//
//		for (int j = 0; j < 36; j++) {
//			cir[i].pt[j].x = cos(PI * j * 10 / 180) * cir[i].rad;
//			cir[i].pt[j].y = sin(PI * j * 10 / 180) * cir[i].rad;
//		}
//	}
//
//	glutPostRedisplay(); //Ÿ�̸ӿ� �ִ´�.
//	glutTimerFunc(100, Timerfunction, 1); //Ÿ�̸� �ٽ� ���
//
//}
//
//void Keyboard(unsigned char key, int x, int y) {
//	switch (key)
//	{
//	case 'q'://������
//
//		break;
//	default:
//		;
//		break;
//	}
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(-1, -1, w, h);
//	glOrtho(0, WideSize, HighSize, 0, 0, 1.0); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
//
//}