#include <gl/freeglut.h>
#include <stdlib.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);

struct position {
	int x;
	int y;
	int z;
};

static position pi;
static position pi2;

void main(int argc, char *argv[])
{
	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// ���÷��� ��� ���� 
	glutInitWindowPosition(100, 100);
	// �������� ��ġ���� 
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ���� 
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape); 
	glutMainLoop();
} // ������ ��� �Լ� 
GLint tri1[3] = { -100, -100,0 };
GLint tri2[3] = { 100, -100,0 };
GLint tri3[3] = { 0, 100,0 };

GLint second1[3] = { 0, -100, -100 };
GLint second2[3] = { 0, -100, 100 };
GLint second3[3] = { 0, 100, 0 };

static int rad = 0;
static int glScaleX = 1, glScaleY = 1;
static int start = 0;

static int smallTri1X = -100, smallTri1Y = -100;
static int smallTri2X = -100, smallTri2Y = -100;

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	glTranslatef(400, 300, 0.0f);
	glRotatef(rad, 0, 1, 0);
	glScaled(glScaleX, glScaleX, 1.0f);
	{
		glBegin(GL_LINES);
		glVertex3iv(tri1);
		glVertex3iv(tri2);
		glEnd();

		glBegin(GL_LINES);
		glVertex3iv(tri2);
		glVertex3iv(tri3);
		glEnd();

		glBegin(GL_LINES);
		glVertex3iv(tri3);
		glVertex3iv(tri1);
		glEnd();
	}

	{
		glBegin(GL_LINES);
		glVertex3iv(second1);
		glVertex3iv(second2);
		glEnd();

		glBegin(GL_LINES);
		glVertex3iv(second2);
		glVertex3iv(second3);
		glEnd();

		glBegin(GL_LINES);
		glVertex3iv(second3);
		glVertex3iv(second1);
		glEnd();
	}
	glPushMatrix();
	glScaled(1, 1, 1);
	{
		glBegin(GL_POLYGON);
		glVertex3d(pi.x - 10, pi.y - 10, 0);
		glVertex3d(pi.x, pi.y + 10, 0);
		glVertex3d(pi.x + 10, pi.y - 10, 0);
		glEnd();
	}

	{
		glBegin(GL_POLYGON);
		glVertex3d(0, pi2.y - 10, pi2.z - 10);
		glVertex3d(0, pi2.y, pi2.z + 10);
		glVertex3d(0, pi2.y + 10, pi2.z - 10);
		glEnd();
	}
	glPopMatrix();
	glPopMatrix();


	glFlush(); // ȭ�鿡 ����ϱ� 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}
static BOOL on = FALSE;
static int dir = 0; 

static int count = 0;
static int triangle = 0;
static int moveX, moveY, moveZ;
void TimerFunction(int value)
{
	glutPostRedisplay();

	if (start == 0) {
		pi.x = tri1[0];
		pi.y = tri1[1];
		pi.z = tri1[2];

		pi2.x = second1[0];
		pi2.y = second1[1];
		pi2.z = second1[2];
		start++;
	}

	if (on == TRUE) {
		if (dir == 1)
			rad += 10;
		else if (dir == 2)
			rad -= 10;
	}

	if (triangle == 0) {
		moveX = (float)(tri2[0] - tri1[0]) / 20;
		moveY = (float)(tri2[1] - tri1[1]) / 20;

		pi.x += moveX;
		pi.y += moveY;

		moveY = (float)(second2[1] - second1[1]) / 20;
		moveZ = (float)(second2[2] - second1[2]) / 20;

		pi2.y += moveY;
		pi2.z += moveZ;
		count++;

		if (count == 20) {
			count = 0;
			triangle = 1;
		}
	}

	else if (triangle == 1) {
		moveX = (float)(tri3[0] - tri2[0]) / 20;
		moveY = (float)(tri3[1] - tri2[1]) / 20;

		pi.x += moveX;
		pi.y += moveY;

		moveY = (float)(second3[1] - second2[1]) / 20;
		moveZ = (float)(second3[2] - second2[2]) / 20;

		pi2.y += moveY;
		pi2.z += moveZ;
		count++;

		if (count == 20) {
			count = 0;
			triangle = 2;
		}
	}

	else if (triangle == 2) {
		moveX = (float)(tri1[0] - tri3[0]) / 20;
		moveY = (float)(tri1[1] - tri3[1]) / 20;

		pi.x += moveX;
		pi.y += moveY;

		moveY = (float)(second1[1] - second3[1]) / 20;
		moveZ = (float)(second1[2] - second3[2]) / 20;

		pi2.y += moveY;
		pi2.z += moveZ;
		count++;

		if (count == 20) {
			count = 0;
			triangle = 0;
		}
	}
	glutTimerFunc(100, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'y':
		dir = 1;
		if (on == FALSE)
			on = TRUE;
		else if (on == TRUE)
			on = FALSE;
		break;

	case 'Y':
		dir = 2;
		if (on == FALSE)
			on = TRUE;
		else if (on == TRUE)
			on = FALSE;
		break;


	case 's':
		glScaleX += 1;
		glScaleY += 1;
		break;

	case 'S':
		glScaleX -= 1;
		glScaleY -= 1;
		break;
	}
}



// ������ 13�� �ڵ�
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
//	float x;
//	float y;
//	float z;
//	float degree;
//};
//
//#define PT 100//���� ���� ����
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
//};
//
//int click_count;
//int save_count;
//int st_help;
//BOOL Save = false;
//BOOL ani = FALSE;
//BOOL Look = FALSE;
//Shape sp1[PT];
//Shape small[PT];
//Shape tra;
//
//void main(int argc, char *argv[]) {
//	//�ʱ�ȭ
//	st_help = 0;
//	for (int i = PT / 3 * 0; i < PT / 3 * 1; i++) {
//		sp1[i].pos.x = st_help * 5;
//		sp1[i].pos.y = st_help * 10 - 150;
//		st_help++;
//	}
//	st_help = 0;
//	for (int i = PT / 3 * 1; i < PT / 3 * 2; i++) {
//		sp1[i].pos.x = -st_help * 10 + sp1[PT / 3 * 1 - 1].pos.x;
//		sp1[i].pos.y = sp1[PT / 3 * 1 - 1].pos.y;
//		st_help++;
//	}
//	st_help = 0;
//	for (int i = PT / 3 * 2; i < PT / 3 * 3; i++) {
//		sp1[i].pos.x = st_help * 5 + sp1[PT / 3 * 2 - 1].pos.x;
//		sp1[i].pos.y = -st_help * 10 + sp1[PT / 3 * 2 - 1].pos.y;
//		st_help++;
//	}
//	//���� �ﰢ��
//	st_help = 0;
//	for (int i = PT / 3 * 0; i < PT / 3 * 1; i++) {
//		small[i].pos.x = st_help * 1;
//		small[i].pos.y = st_help * 1;
//		st_help++;
//	}
//	st_help = 0;
//	for (int i = PT / 3 * 1; i < PT / 3 * 2; i++) {
//		small[i].pos.x = -st_help * 2 + small[PT / 3 * 1 - 1].pos.x;
//		small[i].pos.y = small[PT / 3 * 1 - 1].pos.y;
//		st_help++;
//	}
//	st_help = 0;
//	for (int i = PT / 3 * 2; i < PT / 3 * 3; i++) {
//		small[i].pos.x = st_help * 1 + small[PT / 3 * 2 - 1].pos.x;
//		small[i].pos.y = -st_help * 1 + small[PT / 3 * 2 - 1].pos.y;
//		st_help++;
//	}
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);//������ ���� ��ǥ
//	glutInitWindowSize(WideSize, HighSize); //������ ���� ũ��
//	glutCreateWindow("ex11");
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
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glLineWidth(2);
//	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
//	/*glBegin(GL_LINES);
//	glVertex2i(0, HighSize / 2);
//	glVertex2i(WideSize, HighSize / 2);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glVertex2i(WideSize / 2, 0);
//	glVertex2i(WideSize / 2, HighSize);
//	glEnd();*/
//
//	glMatrixMode(GL_MODELVIEW);
//	//��� ����
//	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
//	//��ǥ�� �׸���
//
//	glPushMatrix(); //���� ���� ����
//
//
//	glPushMatrix();
//	glTranslated(WideSize / 2, HighSize / 2, 0);
//	glRotatef(0 + tra.rot.degree, 0, 1, 0);
//	glBegin(GL_POINTS);
//	for (int i = 0; i < PT; i++) {
//		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);
//
//	}
//	glEnd();
//
//	glPushMatrix();
//	glRotatef(90 + tra.rot.degree, 0, 1, 0);
//	glBegin(GL_POINTS);
//	for (int i = 0; i < PT; i++) {
//		glVertex3i(sp1[i].pos.x, sp1[i].pos.y, sp1[i].pos.z);
//
//	}
//	glEnd();
//	glPopMatrix();
//
//	//----------���� �ﰢ��
//	glPushMatrix(); //���� ���� ����
//	for (int i = 0; i < PT; i++) {
//		glPushMatrix();
//		glTranslated(sp1[PT / 3 * 0].pos.x, sp1[PT / 3 * 0].pos.y, 0);
//		glRotatef(0 + tra.rot.degree, 0, 1, 0);
//		glTranslated(small[i].move.x, small[i].move.y, 0);
//		glBegin(GL_POINTS);
//		glVertex3i(small[i].pos.x, small[i].pos.y, small[i].pos.z);
//		glEnd();
//		glPopMatrix();
//	}
//	glPopMatrix();
//
//	glPushMatrix();
//	glRotatef(90 + tra.rot.degree, 0, 1, 0);
//	glBegin(GL_POINTS);
//	for (int i = 0; i < PT; i++) {
//		glVertex3i(small[i].pos.x, small[i].pos.y, small[i].pos.z);
//
//	}
//	glEnd();
//
//	glPopMatrix();
//	glPopMatrix(); //���� ���� �ݱ�
//
//
//
//	glPopMatrix(); //���� ���� �ݱ�
//
//
//	glutSwapBuffers();
//}
//
//void Mouse(int button, int state, int x, int y) {
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//
//	}
//}
//void Timerfunction(int value) {
//
//	//Ÿ�̸� ���� �Է�
//
//	//�������� ������������ �Ÿ��� 100���� ������
//	//���Ѵ� �� �ƴѰ�?
//	//�������� ������ ������ �Ÿ��� 100���� ������. -> movex��� �̵������� ���� �����.
//
//	tra.move.x = (float)((sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x) / 100);
//	tra.move.y = (float)((sp1[PT / 3 * 1].pos.y - sp1[PT / 3 * 0].pos.y) / 100);
//
//
//	for (int i = 0; i < PT; i++) {
//
//		if (small[i].move.x < (sp1[PT / 3 * 1].pos.x - sp1[PT / 3 * 0].pos.x)) {
//			small[i].move.x += tra.move.x;
//			small[i].move.y += tra.move.y;
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
//	case 'y':
//		if (tra.rot.degree == 360) {
//			tra.rot.degree = 0;
//		}
//		tra.rot.degree -= 2;
//
//		break;
//	case 'Y':
//		if (tra.rot.degree == 360) {
//			tra.rot.degree = 0;
//		}
//		tra.rot.degree += 2;
//
//		break;
//
//	default:
//		;
//		break;
//	}
//}
//
//GLvoid Reshape(int w, int h)
//{
//	glViewport(-1, -1, w, h);
//	glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //�����츦 �ʱ�ȭ �ϴ� �Լ��Դϴ�!
//
//}