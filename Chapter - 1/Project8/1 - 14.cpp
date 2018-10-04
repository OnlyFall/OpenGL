#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);


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

static int count = 0;
static int temp = 0;
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


GLvoid drawScene(GLvoid)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i < temp; ++i) {
		glColor4f((float)(type[i].r / 255), (float)(type[i].g / 255), (float)(type[i].b / 255), 1.0f);
		glPushMatrix();
		if (type[i].size > 0) {

			if (type[i].shape == 1) {
				glTranslatef(type[i].x, type[i].y, 0.0f);
				glRotatef(type[i].radian, 1, 1, 1);
				glutWireCone(type[i].size, type[i].size, 5, 5);
			}

			else if (type[i].shape == 2) {
				glTranslated(type[i].x, type[i].y, 0.0f);
				glRotatef(type[i].radian, 1, 1, 1);
				glutWireCube(type[i].size);
			}

			else if (type[i].shape == 3) {
				glTranslated(type[i].x, type[i].y, 0.0f);
				glRotatef(type[i].radian, 1, 1, 1);
				glutWireSphere(type[i].size, 20, 20);
			}

			else if (type[i].shape == 4) {
				glTranslated(type[i].x, type[i].y, 0.0f);
				glRotatef(type[i].radian, 1, 1, 1);
				glutWireTorus(type[i].size, type[i].size + 10, 20, 20);
			}

			else if (type[i].shape == 5) {
				glTranslated(type[i].x, type[i].y, 0.0f);
				glRotatef(type[i].radian, 1, 1, 1);
				glutWireTeapot(type[i].size);
			}
		}
		glPopMatrix();
	}

	glutSwapBuffers();
//	glFlush(); // 화면에 출력하기 
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
	for (int i = 0; i < temp; ++i) {
		if (dir == 1)
			type[i].radian += 10;
		else if (dir == 2)
			type[i].radian -= 10;

		if(type[i].size > 0)
			type[i].size -= 1;
	}

	glutTimerFunc(100, TimerFunction, 1);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		if (dir == 1)
			dir = 2;
		else if (dir == 2)
			dir = 1;
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}

static BOOL check = FALSE;

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		type[count].shape = rand() % 5 + 1;
	//	type[count].shape = 1;
		type[count].x = x;
		type[count].y = 600 - y - 1;
		type[count].size = 100;
		type[count].radian = 0;

		type[count].r = rand() % 255 + 1;
		type[count].g = rand() % 255 + 1;
		type[count].b = rand() % 255 + 1;

		count++;
		if (check == FALSE)
			temp++;

		if (count == 20) {
			count = 0;
			temp = 20;
			check = TRUE;
		}
	}
	glutPostRedisplay();
}