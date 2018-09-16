//#include <gl/freeglut.h> 
//#include <math.h>
//
//#define pointArrSize 10
//
//GLvoid drawScene(GLvoid);
//
//GLvoid Reshape(int w, int h);
//
//void TimerFunction(int value);
//void Mouse(int button, int state, int x, int y);
//static int count = 0;
//
//struct vertex {
//	int x;
//	int y;
//};
//
//struct circle {
//	int x;
//	int y;
//	double radius = { 0.0 };
//	double Rotation = { 0.0 };
//	double Rotation2 = { 0.0 };
//	double tempX;
//	double tempY;
//	bool check = false;
//	int dir;
//};
//
//static circle ci[100];
//
//
//void main(int argc, char *argv[])
//{
//	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
//	// ���÷��� ��� ���� 
//	glutInitWindowPosition(0, 0);
//	// �������� ��ġ���� 
//	glutInitWindowSize(1920, 1080); // �������� ũ�� ����
//
//	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
//	glutDisplayFunc(drawScene); // ��� �Լ��� ���� 
//	glutReshapeFunc(Reshape);
//	glutMouseFunc(Mouse);
//	glutTimerFunc(10, TimerFunction, 1);
//	glutMainLoop();
//} // ������ ��� �Լ� 
//
//
//GLvoid drawScene(GLvoid)
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW
//
//	for (int i = 0; i < count; i++) {
//		glBegin(GL_POINTS);
//		if (ci[i].dir == 0) {
//			for (double j = 0; j < ci[i].Rotation; j += 0.1) {
//				glVertex2f(ci[i].x + ci[i].radius * cos(j), ci[i].y + ci[i].radius * sin(j));
//				ci[i].radius += 0.5;
//				ci[i].tempX = ci[i].radius * cos(j);
//				ci[i].tempY = ci[i].radius * sin(j);
//			}
//
//			if (ci[i].check == true) {
//				for (double k = -1.4; k < ci[i].Rotation2; k += 0.1) {
//					glVertex2f(ci[i].x + ci[i].tempX * 2 + ci[i].radius * sin(k), ci[i].y + ci[i].radius * cos(k));
//					ci[i].radius -= 0.5;
//				}
//			}
//		}
//
//		else if (ci[i].dir == 1) {
//			for (double j = 0; j < ci[i].Rotation; j += 0.1) {
//				glVertex2f(ci[i].x + ci[i].radius * sin(j), ci[i].y + ci[i].radius * cos(j));
//				ci[i].radius += 0.5;
//				ci[i].tempX = ci[i].radius * cos(19);
//				ci[i].tempY = ci[i].radius * sin(19);
//			}
//
//			if (ci[i].check == true) {
//				for (double k = -3.3; k < ci[i].Rotation2 - 3.3; k += 0.1) {
//					glVertex2f(ci[i].x + ci[i].tempX * 2 + ci[i].radius * cos(k), ci[i].y + ci[i].radius * sin(k));
//					ci[i].radius -= 0.5;
//				}
//			}
//		}
//
//		ci[i].radius = 0.1;
//		glEnd();
//	}
//
//	glFlush(); // ȭ�鿡 ����ϱ� 
//}
//
//GLvoid Reshape(int w, int h)    // �ٽ� �׸��� �Լ�
//{
//	glViewport(-1, -1, w, h);
//	glOrtho(0, 1920, 1080, 0, 0, 1);
//}
//
//void Mouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		ci[count].x = x;
//		ci[count].y = y;
//		ci[count].dir = rand() % 2;
//		count++;
//	}
//}
//
//void TimerFunction(int value)
//{
//	glutPostRedisplay();
//	for (int i = 0; i < count; i++) {
//		if (ci[i].dir == 0) {
//			if (ci[i].Rotation < 19.0)
//				ci[i].Rotation += 0.5;
//			else {
//				ci[i].check = true;
//				if (ci[i].Rotation2 < 19.0)
//					ci[i].Rotation2 += 0.5;
//			}
//		}
//		else if (ci[i].dir == 1) {
//			if (ci[i].Rotation < 20.5)
//				ci[i].Rotation += 0.5;
//			else {
//				ci[i].check = true;
//				if (ci[i].Rotation2 < 19.0)
//					ci[i].Rotation2 += 0.5;
//			}
//		}
//	}
//	
//	glutTimerFunc(100, TimerFunction, 1);
//}















// �׽�Ʈ ���� 8�� �ڵ�
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
//
//				  //���콺 ����
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
//#define cir_amount 15
//#define PI 3.14159265358
//
//typedef struct Circle {
//	POINT pt[60]; //360��
//	POINT pt2[60];
//	Color cl; //����
//	int rad[60];//������
//	int Ani_rad; //�ִϸ��̼� ����
//	POINT pos;//�߽� ��ǥ
//	int Max_rad; //�ƽ� ����
//				 //-------------------------------------
//	BOOL Ani;//������ ������ �ִϸ��̼� ����
//	int Time_count; //Ÿ�̸� ī��Ʈ
//
//	int Ani1_count;//������ ������
//	int Ani2_count;//������ ������
//};
//int Ani_rand;
//BOOL draw;
//int circle_count = 0;
//int draw_count = 0;
//Circle cir[cir_amount];
//
//void main(int argc, char *argv[]) {
//	//�ʱ�ȭ
//	for (int i = 0; i < cir_amount; i++) {
//
//		//   cir[i].cl.R = 255;
//		//   cir[i].cl.G = 255;
//		//   cir[i].cl.B = 255;
//		for (int k = 0; k < 60; k++) {
//			cir[i].rad[k] = 0;
//		}
//		cir[i].Ani = FALSE;
//
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
//		for (int j = 0; j < 60; j++) {
//			glVertex2f(
//				cir[i].pos.x + cir[i].pt[j].x,
//				cir[i].pos.y + cir[i].pt[j].y
//			);
//		}
//		for (int j = 59; j >= 0; j--) {
//			if (cir[i].Ani == TRUE)
//				glVertex2f(
//					cir[i].pos.x + cir[i].pt2[j].x,
//					cir[i].pos.y + cir[i].pt2[j].y
//				);
//		}
//
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
//
//
//		cir[circle_count].pos.x = x;
//		cir[circle_count].pos.y = y;
//
//		cir[circle_count].cl.R = 100;
//		cir[circle_count].cl.G = 100;
//		cir[circle_count].cl.B = 200;
//
//		for (int k = 0; k < 60; k++) {
//			cir[circle_count].rad[k] = 0;
//		}
//		//circle ī��Ʈ �ʱ�ȭ
//		if (circle_count == 19) {
//			circle_count = 0;
//			draw = TRUE;
//		}
//		else {
//			if (draw)
//				draw_count = 20;
//			else
//				draw_count++;
//
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
//	for (int i = 0; i < draw_count; i++) {
//
//
//		if (cir[i].Ani) {//�ִϸ��̼��� Ʈ����
//			;
//		}
//
//		cir[i].Time_count++;
//		if (cir[i].Time_count % 1 == 0) {
//			if (cir[i].Ani1_count <= 60)
//			{
//				cir[i].Ani = FALSE;
//				cir[i].Ani1_count++;
//
//			}
//			else {
//				cir[i].Ani = TRUE;
//			}
//
//			if (cir[i].Ani2_count <= 60 && cir[i].Ani == TRUE) {
//				cir[i].Ani2_count++;
//				for (int j = 0; j < cir[i].Ani2_count; j++) {
//					cir[i].rad[59 - j] = j * 1.5; //������ ����
//					cir[i].pt2[59 - j].x = -cos(PI * j * 24 / 180) * cir[i].rad[59 - j] + cir[i].rad[59] * 2;
//					cir[i].pt2[59 - j].y = sin(PI * j * 24 / 180) * cir[i].rad[59 - j];
//
//				}
//			}
//
//		}
//
//		for (int j = 0; j < cir[i].Ani1_count; j++) {
//			cir[i].rad[j] = j * 1.5; //������ ����
//
//			cir[i].pt[j].x = cos(PI * j * 24 / 180) * cir[i].rad[j];
//			cir[i].pt[j].y = -sin(PI * j * 24 / 180) * cir[i].rad[j];
//
//
//
//		}
//
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