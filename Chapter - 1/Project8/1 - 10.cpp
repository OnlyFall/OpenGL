#include <gl/freeglut.h> 
#include <math.h>

#define pointArrSize 10

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);
void Motion(int x, int y);
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
static int count = 0;
static bool mouseMove = false;
struct rectangle {
	int x;
	int y;
	int size = 10;

	int moveX = 0;
	float r = 100;
	float g = 32;
	float b = 213;

	int speed;

	bool rBool = false;
	bool gBool = false;
	bool bBool = false;
	bool xBool = false;
	bool yBool = false;
	bool check = false;
	bool movecheck = true;
};

struct DieRectangle {
	int x;
	int y;
	int size = 20;
	bool visible = false;
};

static rectangle rc[100];
static DieRectangle DR;

void main(int argc, char *argv[])
{
	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// 디스플레이 모드 설정 
	glutInitWindowPosition(0, 0);
	// 윈도우의 위치지정 
	glutInitWindowSize(1920, 1080); // 윈도우의 크기 지정

	for (int i = 0; i < 30; ++i) {
		rc[i].x = rand() % 1600 + 200;
		rc[i].y = rand() % 1000 + 30;
		rc[i].speed = rand() % 5 + 2;
	}

	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
} // 윈도우 출력 함수 


GLvoid drawScene(GLvoid)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW
	for (int i = 0; i < 30; ++i) {
		if (rc[i].check == false) {
			glBegin(GL_POLYGON);
			glColor4f((float)rc[i].r / 255, (float)rc[i].g / 255, (float)rc[i].b / 255, 1.0f);
			glVertex2d(rc[i].x - rc[i].size, rc[i].y - rc[i].size);
			glVertex2d(rc[i].x - rc[i].size, rc[i].y + rc[i].size);
			glVertex2d(rc[i].x + rc[i].size, rc[i].y + rc[i].size);
			glVertex2d(rc[i].x + rc[i].size, rc[i].y - rc[i].size);
			glEnd();
		}
		else if (rc[i].check == true) {
			glBegin(GL_POLYGON);
			glColor4f((float)rc[i].r / 255, (float)rc[i].g / 255, (float)rc[i].b / 255, 1.0f);
			glVertex2d(rc[i].x - rc[i].size + rc[i].moveX , rc[i].y - rc[i].size);
			glVertex2d(rc[i].x - rc[i].size, rc[i].y + rc[i].size);
			glVertex2d(rc[i].x + rc[i].size , rc[i].y + rc[i].size);
			glVertex2d(rc[i].x + rc[i].size - rc[i].moveX, rc[i].y - rc[i].size);
			glEnd();
		}
	}

	if (DR.visible == true) {
		glBegin(GL_POLYGON);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex2d(DR.x - DR.size, DR.y - DR.size);
		glVertex2d(DR.x - DR.size, DR.y + DR.size);
		glVertex2d(DR.x + DR.size, DR.y + DR.size);
		glVertex2d(DR.x + DR.size, DR.y - DR.size);
		glEnd();
	}

	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)    // 다시 그리기 함수
{
	glViewport(-1, -1, w, h);


	glOrtho(0, 1920, 1080, 0, 0, 1);
}

void Mouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)) {
		DR.x = x;
		DR.y = y;

		for (int i = 0; i < 30; ++i) {
			if (DR.x - DR.size <= rc[i].x + rc[i].size && DR.x + DR.size >= rc[i].x - rc[i].size
				&& DR.y - DR.size <= rc[i].y + rc[i].size && DR.y + DR.size >= rc[i].y - rc[i].size)
				rc[i].check = true;
		}
		mouseMove = true;
		DR.visible = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouseMove = false;
		DR.visible = false;
	}
}

void Motion(int x, int y)
{
	if (mouseMove == true) {
		DR.x = x;
		DR.y = y;

		for (int i = 0; i < 30; ++i) {
			if (DR.x - DR.size <= rc[i].x + rc[i].size && DR.x + DR.size >= rc[i].x - rc[i].size
				&& DR.y - DR.size <= rc[i].y + rc[i].size && DR.y + DR.size >= rc[i].y - rc[i].size && rc[i].check == false) {
				rc[i].check = true;
				rc[i].size += 20;
			}
		}
	}
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	for (int i = 0; i < 30; ++i) {
		if (rc[i].rBool == false) {
			rc[i].r += 1;
			if (rc[i].r == 255)
				rc[i].rBool = true;
		}
		else if (rc[i].rBool == true) {
			rc[i].r -= 1;
			if (rc[i].r == 0)
				rc[i].rBool = false;
		}

		if (rc[i].gBool == false) {
			rc[i].g += 1;
			if (rc[i].g == 255)
				rc[i].gBool = true;
		}
		else if (rc[i].gBool == true) {
			rc[i].g -= 1;
			if (rc[i].g == 0)
				rc[i].gBool = false;
		}

		if (rc[i].bBool == false) {
			rc[i].b += 1;
			if (rc[i].b == 255)
				rc[i].bBool = true;
		}
		else if (rc[i].bBool == true) {
			rc[i].r -= 1;
			if (rc[i].b == 0)
				rc[i].bBool = false;
		}
	}

	for (int i = 0; i < 30; ++i) {
		if (rc[i].xBool == false)
			rc[i].x -= rc[i].speed;
		else if (rc[i].xBool == true)
			rc[i].x += rc[i].speed;

		if (rc[i].yBool == false)
			rc[i].y -= rc[i].speed;
		else if (rc[i].yBool == true)
			rc[i].y += rc[i].speed;

		if (rc[i].x - rc[i].size - rc[i].speed < 0)
			rc[i].xBool = true;
		else if (rc[i].x + rc[i].size + rc[i].speed > 1920)
			rc[i].xBool = false;

		if (rc[i].y - rc[i].size - rc[i].speed < 0)
			rc[i].yBool = true;
		else if (rc[i].y + rc[i].size + rc[i].speed > 1080)
			rc[i].yBool = false;

		if (rc[i].check == true) {
			if (rc[i].movecheck == true) {
				rc[i].moveX += 1;
				if (rc[i].moveX == rc[i].size)
					rc[i].movecheck = false;
			}
			else if (rc[i].movecheck == false) {
				rc[i].moveX -= 1;
				if (rc[i].moveX == 0) {
					rc[i].check = false;
					rc[i].movecheck = true;
					rc[i].size -= 20;
				}
			}
	

		}

	}
	glutTimerFunc(10, TimerFunction, 1);
}




