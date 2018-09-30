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
	// 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100);
	// 윈도우의 위치지정 
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape); 
	glutMainLoop();
} // 윈도우 출력 함수 
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
	glScaled(glScaleX, glScaleY, 1.0f);
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


	glFlush(); // 화면에 출력하기 
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