#include <gl/freeglut.h> 
#include <math.h>

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);

static int shape = 0;

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

	vertex circleVertex[200];
	int in;
	int count = 0;
	double radius = { 0.0 };
	double Rotation = 19;
	bool out = false;
};

struct Shape {
	double x = 0;
	double y = 0;
	double z = 0;

	double moveX = 0;
	double moveY = 0;
	double moveZ = 0;
	double Rotation = 19;
	int count = 0;
};

struct zigzag {
	int x;
	int y;
};

static zigzag zig[9];
static Shape rect[800];
static circle ci[100];
static int rad = 0;
static int yRotate = 0;

static vertex input[5];
static int inputCount = 0;
static BOOL inputDraw = FALSE;

static float moveX;
static float moveY;

static BOOL radBool = FALSE;

static float rectleftX, rectleftY;
static float rectrightX, rectrightY;
static int count = 0;
static int wrong;
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
	glutMouseFunc(Mouse);
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
} // 윈도우 출력 함수 
static int size = 20;
static int circleShape = 1;
static float triCenterX, triCenterY;

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//기본 직선들 그리는 장소
	GLint p1[2] = { 0, 300 };
	GLint p2[2] = { 800, 300 };
	GLint p3[2] = { 400, 0 };
	GLint p4[2] = { 400, 600 };

	glPushMatrix();
	glTranslatef(400, 300, 0.0f);
	glRotatef(yRotate, 0, 1, 0);
	glTranslatef(-400, -300, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2iv(p1);
	glVertex2iv(p2);
	glVertex2iv(p3);
	glVertex2iv(p4);
	glEnd();

	//=====여긴 그래프부터 그림==========
	if (shape == 1) {
		glColor4f((float)255, (float)255, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 36; ++i) {

			glVertex2d(ci[0].circleVertex[i].x, ci[0].circleVertex[i].y);
		}
		glEnd();
	}

	else if (shape == 2) {
		glBegin(GL_LINES);
		for (int i = 0; i < 800; ++i) {
			glVertex2f(rect[i].x, rect[i].y);
		}

		glEnd();
	}

	else if (shape == 3) {
		glBegin(GL_POINTS);
		for(int i = 0; i < wrong; ++i)
			glVertex2f(ci[0].circleVertex[i].x, ci[0].circleVertex[i].y);
		glEnd();
	}

	else if (shape == 4) {
		for (int i = 0; i < 8; ++i) {
			glBegin(GL_LINES);
			glVertex2d(zig[i].x, zig[i].y);
			glVertex2d(zig[i + 1].x, zig[i + 1].y);
			glEnd();
		}
	}

	else if (shape == 5) {
		if (inputDraw == TRUE) {
			glBegin(GL_LINES);
			for (int i = 0; i < inputCount - 1; ++i) {
				glVertex2f(input[i].x, input[i].y);
				glVertex2f(input[i + 1].x, input[i + 1].y);
			}
			glEnd();
		}
	}

	//======여긴 뭔가 빙빙돔=============
	if (shape == 1) {
		glPushMatrix();
		if (circleShape == 1) {
			glTranslated(rectleftX - size, rectleftY - size, 0);
			glRotated(rad, 0, 0, 1);
			glTranslated((rectleftX - size) * -1, (rectleftY - size) * -1, 0);
		}
		else if (circleShape == 2) {
			glTranslated(triCenterX, triCenterY, 0);
			glRotated(rad, 0, 0, 1);
			glTranslated(triCenterX * -1, triCenterY * -1, 0);
		}
	}
	glColor4f((float)255, (float)0, 0.0f, 1.0f);

	if(circleShape == 1)
		glRectf(rectleftX + size, rectleftY + size, rectrightX - size, rectrightY - size);
	
	else if (circleShape == 2) {
		glBegin(GL_TRIANGLES);
		glVertex2f(triCenterX - size, triCenterY - size);
		glVertex2f(triCenterX + size, triCenterY - size);
		glVertex2f(triCenterX, triCenterY + size);
		glEnd();
	}

	if (shape == 1)
		glPopMatrix();

	glPopMatrix();

	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

static int temp = 0;
static int dir = 0;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		count = 0;
		shape = 1;
		rad = 0;
		ci[0].x = 400;
		ci[0].y = 300;
		for (int j = 0; j < 36; ++j) {
			ci[0].circleVertex[j].x = cos((3.141592 / 180.f) * 10 * (j + 1)) * 150 + ci[0].x;
			ci[0].circleVertex[j].y = sin((3.141592 / 180.f) * 10 * (j + 1)) * 150 + ci[0].y;
		}
		break;

	case '2':
		count = 0;
		shape = 2;
		for (int i = 0; i < 800; ++i) {
			rect[i].x = i + rect[i].moveX;
			rect[i].y = 100 * sin((rect[i].x / 42.5)) + 300 + rect[i].moveY;
		}
		break;

	case '3':
		shape = 3;
		wrong = 0;
		count = 0;
		ci[0].x = 400;
		ci[0].y = 300;
		ci[0].radius = 0.5;
		for (double j = 0; j < 20.0; j += 0.1) {
			ci[0].circleVertex[wrong].x = ci[0].x + ci[0].radius * cos(j);
			ci[0].circleVertex[wrong].y = ci[0].y + ci[0].radius * sin(j);
			ci[0].radius += 0.5;
			wrong++;
		}
		break;

	case '4':
		shape = 4;
		count = 0;
		temp = 0;

		for (int i = 0; i < 9; ++i) {
			zig[i].x = i * 100;
			if (i % 2 == 0)
				zig[i].y = 300 + 200;
			else
				zig[i].y = 300 - 200;
		}

		rectleftX = zig[0].x;
		rectleftY = zig[0].y;
		rectrightX = zig[0].x;
		rectrightY = zig[0].y;

		triCenterX = zig[0].x;
		triCenterY = zig[0].y;


		break;

	case '5':
		shape = 5;
		inputCount = 0;
		count = 0;
		inputDraw = FALSE;
		break;

	case 'c':
		if (circleShape == 1)
			circleShape = 2;
		else if (circleShape == 2)
			circleShape = 1;

		break;

	case 'y':
		if (radBool == FALSE)
			radBool = TRUE;
		else if (radBool == TRUE)
			radBool = FALSE;

		dir = 1;
		break;

	case 'Y':
		if (radBool == FALSE)
			radBool = TRUE;
		else if (radBool == TRUE)
			radBool = FALSE;

		dir = 2;
		break;
	}
}


static BOOL check = FALSE;
void TimerFunction(int value)
{
	glutPostRedisplay();
	
	if (shape == 1) {
		rad += 10;
		if (circleShape == 1) {
			rectleftX = ci[0].circleVertex[count].x;
			rectleftY = ci[0].circleVertex[count].y;
			rectrightX = ci[0].circleVertex[count].x;
			rectrightY = ci[0].circleVertex[count].y;
		}
		else if (circleShape == 2) {
			triCenterX = ci[0].circleVertex[count].x;
			triCenterY = ci[0].circleVertex[count].y;
		}
		count++;
		if (count == 36)
			count = 0;

	}

	else if (shape == 2) {
		if (circleShape == 1) {
			rectleftX = rect[count].x;
			rectleftY = rect[count].y;
			rectrightX = rect[count].x;
			rectrightY = rect[count].y;
		}

		else if (circleShape == 2) {
			triCenterX = rect[count].x;
			triCenterY = rect[count].y;
		}

		count++;
		if (count == 800)
			count = 0;
	}

	else if (shape == 3) {
		if (circleShape == 1) {
			rectleftX = ci[0].circleVertex[count].x;
			rectleftY = ci[0].circleVertex[count].y;
			rectrightX = ci[0].circleVertex[count].x;
			rectrightY = ci[0].circleVertex[count].y;
		}
		else if (circleShape == 2) {
			triCenterX = ci[0].circleVertex[count].x;
			triCenterY = ci[0].circleVertex[count].y;
		}
		count++;
		if (count == wrong)
			count = 0;
	}

	else if (shape == 4) {
		moveX = (float)(zig[count + 1].x - zig[count].x) / 10;
		moveY = (float)(zig[count + 1].y - zig[count].y) / 10;
		
		rectleftX += moveX;
		rectleftY += moveY;
		rectrightX += moveX;
		rectrightY += moveY;

		triCenterX += moveX;
		triCenterY += moveY;
		temp++;
		if (temp == 10) {
			count++;
			temp = 0;
		}

		if (count == 9) {
			count = 0;
			rectleftX = zig[0].x + size;
			rectleftY = zig[0].y + size;
			rectrightX = zig[0].x - size;
			rectrightY = zig[0].y - size;

			triCenterX = zig[0].x;
			triCenterY = zig[0].y;
		}
	}
	
	else if (shape == 5 && inputDraw == TRUE) {
		moveX = (float)(input[count + 1].x - input[count].x) / 50;
		moveY = (float)(input[count + 1].y - input[count].y) / 50;

		rectleftX += moveX;
		rectleftY += moveY;
		rectrightX += moveX;
		rectrightY += moveY;

		triCenterX += moveX;
		triCenterY += moveY;
		temp++;
		if (temp == 50) {
			count++;
			temp = 0;
		}

		if (count == inputCount - 1) {
			count = 0;
			rectleftX = input[0].x + size;
			rectleftY = input[0].y + size;
			rectrightX = input[0].x - size;
			rectrightY = input[0].y - size;

			triCenterX = input[0].x;
			triCenterY = input[0].y;
		}
	}

	if (check == FALSE) {
		size += 5;
		if (size == 40)
			check = TRUE;
	}

	else if (check == TRUE) {
		size -= 5;
		if (size == 20)
			check = FALSE;
	}

	if (radBool == TRUE) {
		if (dir == 1)
			yRotate += 10;
		else if (dir == 2)
			yRotate -= 10;
	}
	glutTimerFunc(100, TimerFunction, 1);
}


void Mouse(int button, int state, int x, int y)
{
	int tempX;
	int tempY;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && inputCount < 5 && inputDraw == FALSE) {
		input[inputCount].x = x;
		input[inputCount].y = 600 - y - 1;
		inputCount++;

		if (inputCount == 5) {
			inputDraw = TRUE;
			rectleftX = input[0].x + size;
			rectleftY = input[0].y + size;
			rectrightX = input[0].x - size;
			rectrightY = input[0].y - size;

			triCenterX = input[0].x;
			triCenterY = input[0].y;
			glutTimerFunc(1, TimerFunction, 1);
		}
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && inputDraw == FALSE) {
		rectleftX = input[0].x + size;
		rectleftY = input[0].y + size;
		rectrightX = input[0].x - size;
		rectrightY = input[0].y - size;

		triCenterX = input[0].x;
		triCenterY = input[0].y;
		inputDraw = TRUE;
		glutTimerFunc(1, TimerFunction, 1);
	}
}