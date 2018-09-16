#include <GL/glut.h>     // 또는 <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#define RectSize 30
#define WindowXSize 800
#define WindowYSize 600

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);
void TimerFunction(int value);

void Mouse(int button, int state, int x, int y);
static int count = 0;
bool check = false;
struct rect {
	int x;
	int y;
	int size;
	float r;
	float g;
	float b;
	bool rBool = false;
	bool gBool = false;
	bool bBool = false;
	bool xBool = false;
	bool yBool = false;
	bool sizeUpdown = false;
	int count = 0;
};

static rect rectangle[10];
static int total;

void main(int argc, char** argv)    // 윈도우 출력하고 출력함수 설정
{   // 초기화 함수들 
	glutInit(&argc, argv);    // glut 초기화        
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  // 디스플레이 모드 설정        
	glutInitWindowPosition(100, 100);   // 윈도우의 위치 지정        
	glutInitWindowSize(800, 600);    // 윈도우의 크기 지정       
	glutCreateWindow("오예");   // 윈도우 생성 (윈도우 이름)         
	glutDisplayFunc(drawScene);    // 출력 함수의 지정         
	glutReshapeFunc(reshape);   // 다시 그리기 함수 지정     

	glutMouseFunc(Mouse);
	glutTimerFunc(10, TimerFunction, 1);
	srand(time(NULL));
	glutMainLoop();  // 이벤트 처리 시작 

}
GLvoid drawScene()     // 출력 함수 
{
	int r = 200;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (int i = 0; i < total; ++i) {
		glColor4f((float)rectangle[i].r / 255, (float)rectangle[i].g / 255, (float)rectangle[i].b / 255, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2d(rectangle[i].x - rectangle[i].size, rectangle[i].y - rectangle[i].size);
		glVertex2d(rectangle[i].x - rectangle[i].size, rectangle[i].y + rectangle[i].size);
		glVertex2d(rectangle[i].x + rectangle[i].size, rectangle[i].y + rectangle[i].size);
		glVertex2d(rectangle[i].x + rectangle[i].size, rectangle[i].y - rectangle[i].size);
		glEnd();
	}
	
	glFlush();      // 화면에 출력하기


}
GLvoid reshape(int w, int h)    // 다시 그리기 함수
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WindowXSize, WindowYSize, 0, 0, 1);
}

void TimerFunction(int value) {
	glutPostRedisplay(); // 화면 재 출력
	for (int i = 0; i < total; ++i) {
		if (rectangle[i].xBool == false)
			rectangle[i].x -= 3;
		else if (rectangle[i].xBool == true)
			rectangle[i].x += 3;

		if (rectangle[i].yBool == false)
			rectangle[i].y -= 3;
		else if (rectangle[i].yBool == true)
			rectangle[i].y += 3;

		if (rectangle[i].x - rectangle[i].size - 3 < 0)
			rectangle[i].xBool = true;
		else if (rectangle[i].x + rectangle[i].size + 3 > 800)
			rectangle[i].xBool = false;

		if (rectangle[i].y - rectangle[i].size - 3 < 0)
			rectangle[i].yBool = true;
		else if (rectangle[i].y + rectangle[i].size + 3 > 600)
			rectangle[i].yBool = false;

		if (rectangle[i].sizeUpdown == false) {
			rectangle[i].count++;
			rectangle[i].size += 1;
			if (rectangle[i].count == 10) {
				rectangle[i].count = 0;
				rectangle[i].sizeUpdown = true;
			}
		}
		else if (rectangle[i].sizeUpdown == true) {
			rectangle[i].count++;
			rectangle[i].size -= 1;
			if (rectangle[i].count == 10) {
				rectangle[i].count = 0;
				rectangle[i].sizeUpdown = false;
			}
		}

		if (rectangle[i].rBool == false) {
			rectangle[i].r += 1;
			if (rectangle[i].r == 255)
				rectangle[i].rBool = true;
		}
		else if (rectangle[i].rBool == true) {
			rectangle[i].r -= 1;
			if (rectangle[i].r == 0)
				rectangle[i].rBool = false;
		}

		if (rectangle[i].gBool == false) {
			rectangle[i].g += 1;
			if (rectangle[i].g == 255)
				rectangle[i].gBool = true;
		}
		else if (rectangle[i].gBool == true) {
			rectangle[i].g -= 1;
			if (rectangle[i].g == 0)
				rectangle[i].gBool = false;
		}

		if (rectangle[i].bBool == false) {
			rectangle[i].b += 1;
			if (rectangle[i].b == 255)
				rectangle[i].bBool = true;
		}
		else if (rectangle[i].bBool == true) {
			rectangle[i].r -= 1;
			if (rectangle[i].b == 0)
				rectangle[i].bBool = false;
		}




	}
	glutTimerFunc (10, TimerFunction, 1); // 타이머함수 재 설정
}

void Mouse(int button, int state, int x, int y)
{
	int tempX;
	int tempY;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		rectangle[count].x = x;
		rectangle[count].y = y;
		rectangle[count].size = rand() % 40 + 20;
		rectangle[count].r = rand() % 255;
		rectangle[count].g = rand() % 255;
		rectangle[count].b = rand() % 255;
		tempX = rand() % 2;
		tempY = rand() % 2;
		if (tempX == 0)
			rectangle[count].xBool = false;
		else if (tempX == 1)
			rectangle[count].xBool = true;

		if (tempY == 0)
			rectangle[count].yBool = false;
		else if (tempY == 1)
			rectangle[count].yBool = true;

		count++;
		if(check == false)
			total = count;
		if (count >= 10) {
			count %= 10;
			total = 10;
			check = true;
		}
	}

}