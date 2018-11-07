#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

static int count = 0;
static BOOL check = FALSE;
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

static position ControllPoint[20];
GLfloat ctrlpoints[20][3];


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
	int i = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	check = FALSE;
	i = 0;
	while (check != TRUE) {
		if (count > 3) {
			glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[i][0]);

			glEnable(GL_MAP1_VERTEX_3);

			glMapGrid1f(10, 0.0, 1.0);
			glEvalMesh1(GL_LINE, 0, 10);
		}

		glDisable(GL_MAP1_VERTEX_3);
		i += 3;
		if (i >= count && i + 1 >= count || i + 2 >= count || i + 3 >= count)
			check = TRUE;
	}

	glPointSize(5.0);

	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < count; ++i)
		glVertex3fv(&ctrlpoints[i][0]);
	glEnd();

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


	glutTimerFunc(1, TimerFunction, 1);

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



void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ControllPoint[count].x = x;
		ControllPoint[count].y = 600 - 1 - y;
		ctrlpoints[count][0] = x;
		ctrlpoints[count][1] = 600 - 1 - y;
		ctrlpoints[count][2] = 0.0;
		if (count < 20)
			count++;
		else
			count = 0;
	}
	glutPostRedisplay();
}