#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <time.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);



void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100);
	// 윈도우의 위치지정 
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
} // 윈도우 출력 함수 
static int xRad = 30;
static int yRad = 30;

void drawNumber(int number, int moveX, int moveY);
GLvoid drawScene(GLvoid)
{
	time_t now = time(0);
	struct tm curr_time;
	localtime_s(&curr_time, &now);

	int temp_hour;
	int temp_min;
	int temp_sec;
	curr_time.tm_hour;
	curr_time.tm_min;
	curr_time.tm_sec;


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	temp_hour = curr_time.tm_hour / 10;
	drawNumber(curr_time.tm_hour / 10, 60, 300);
	drawNumber(curr_time.tm_hour % 10, 170, 300);

	if(curr_time.tm_sec % 2 == 0)
		drawNumber(17, 170 + ((340 - 170) / 2) - 20, 340);
	else
		drawNumber(18, 170 + ((340 - 170) / 2) - 20, 340);

	drawNumber(curr_time.tm_min / 10, 340, 300);
	drawNumber(curr_time.tm_min % 10, 450, 300);

	if (curr_time.tm_sec % 2 == 0)
		drawNumber(17, 450 + ((620 - 450) / 2) - 10, 340);
	else
		drawNumber(18, 450 + ((620 - 450) / 2) - 10, 340);

	drawNumber(curr_time.tm_sec / 10, 620, 300);
	drawNumber(curr_time.tm_sec % 10, 730, 300);

//	drawNumber(temp_hour % 10, 100, 100);
	glutSwapBuffers();
	//	glFlush(); // 화면에 출력하기 
}

void drawNumber(int number, int moveX, int moveY)
{
	glPushMatrix();
	glTranslated(400, 300, 0);
	glRotated(xRad, 1, 0, 0);
	glRotated(yRad, 0, 1, 0);
	glPushMatrix();
	glTranslated(-400, -300, 0);
	glTranslated(moveX, moveY, 0);
	switch (number) {
	case 17:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glTranslatef(0, 310, 0);
		glTranslatef(0, 310, 0);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.25, 0.25);
		glTranslated(70, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();
		break;

	case 18:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glTranslatef(0, 310, 0);
		glTranslatef(0, 310, 0);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.25, 0.25);
		glTranslated(70, 77, 0);
		glColor4f(0.3f, 0.3f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.3f, 0.3f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();
		break;

	case 1:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);


		glTranslatef(0, 310, 0);


		glTranslatef(0, 310, 0);

		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();

		break;

	case 2:

		glPushMatrix();
		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(275, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();


		break;

	case 3:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();
		break;

	case 4:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);


		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);
		glPopMatrix();

		glPopMatrix();
		break;

	case 5:
		glPushMatrix();


		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPopMatrix();
		break;

	case 6:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glPopMatrix();

		glPopMatrix();
		break;

	case 7:
		glPushMatrix();


		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);


		glTranslatef(0, 310, 0);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);


		glPopMatrix();

		glPopMatrix();
		break;

	case 8:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glPopMatrix();

		glPopMatrix();
		break;

	case 9:
		glPushMatrix();

		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);


		glPopMatrix();

		glPopMatrix();
		break;

	case 0:
		glPushMatrix();


		glPushMatrix();
		glScaled(0.5, 0.25, 0.25);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslatef(0, 310, 0);


		glTranslatef(0, 310, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);
		glPopMatrix();

		glPushMatrix();
		glScaled(0.25, 0.5, 0.25);
		glTranslated(-140 + 275, 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, 235 - 77, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(-275, 0, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glTranslated(0, -158, 0);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0f);
		glutSolidCube(100);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(101);

		glPopMatrix();

		glPopMatrix();
		break;

	}
	glPopMatrix();
	glPopMatrix();
}

void drawMin()
{

}

void drawSec()
{

}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

static int dir = 1;
void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1, TimerFunction, 1);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		xRad += 10;
		break;

	case 'X':
		xRad -= 10;
		break;

	case 'y':
		yRad += 10;
		break;

	case 'Y':
		yRad -= 10;
		break;
	}
}

static BOOL check = FALSE;

void Mouse(int button, int state, int x, int y)
{

	glutPostRedisplay();
}




































//#include <GL/glut.h>
//#include <time.h>
//
//#define WINDOW_WIDTH 500
//#define WINDOW_HEIGHT 500
//
//void draw_arm(float angle, float length)
//{
//	glPushMatrix();
//	glRotatef(-angle, 0.0f, 0.0f, 1.0f);
//	glScalef(10 * length, 1.0f, 1.0f);//시계침의 길이는 정확하지 않음
//	if (length == 1.0f)
//		glColor3f(1.0, 0.0, 0.0);
//	if (length == 1.2f)
//		glColor3f(0.0, 1.0, 0.0);
//	if (length == 1.4f)
//		glColor3f(0.0, 0.0, 1.0);
//	glBegin(GL_POLYGON);
//	glVertex3f(-0.1, 0.0, 0.0);
//	glVertex3f(0.0, 0.02, 0.0);
//	glVertex3f(0.0, -0.02, 0.0);
//	glEnd();
//	glPopMatrix();
//}
//
//void tick_mark()
//{
//	glColor3f(0.0, 1.0, 1.0);
//	glBegin(GL_POLYGON);
//	glVertex3f(-0.1, 0.0, 0.0);
//	glVertex3f(0.0, 0.02, 0.0);
//	glVertex3f(0.0, -0.02, 0.0);
//	glEnd();
//}
//
//void draw_plate()
//{
//	for (int i = 0; i < 60; i++)
//	{
//		glPushMatrix();
//
//		glRotatef(i*6.0f, 0.0f, 0.0f, -1.0f);
//		glTranslatef(1.55f, 0.0f, 0.0f);
//
//		if (i % 5 == 0)
//			glScalef(2.0f, 1.0f, 1.0f);
//
//		tick_mark();
//
//		glPopMatrix();
//	}
//}
//
//void reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	if (w <= h)
//		gluOrtho2D(-2.0, 2.0, -2.0*(GLfloat)h / (GLfloat)w, 2.0*(GLfloat)h / (GLfloat)w);
//	else
//		gluOrtho2D(-2.0*(GLfloat)w / (GLfloat)h, 2.0*(GLfloat)w / (GLfloat)h, -2.0, 2.0);
//	//gluOrtho2D (-2.0, 2.0, -2.0, 2.0);
//
//}
//
//void display()
//{
//	struct tm nowtime;
//	time_t t;
//	float hour, minute, second;
//
//	time(&t);//현재 시간을 알아옴
//
//	localtime_s(&nowtime, &t);//시간 구조체로 변환
//
//	hour = (nowtime.tm_hour % 12)*30.0 + 90;
//	minute = nowtime.tm_min*6.0 + 90;
//	second = nowtime.tm_sec*6.0 + 90;
//
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	draw_plate();
//	draw_arm(hour, 1.0f);
//	draw_arm(minute, 1.2f);
//	draw_arm(second, 1.4f);
//
//	glFlush();
//}
//
//void idle()
//{
//	display();
//}
//
//void init()
//{
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("Reshape CALLBACK");
//	init();
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutIdleFunc(idle);
//	glutMainLoop();
//	return 0;
//}