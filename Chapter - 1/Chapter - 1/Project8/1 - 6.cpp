#include <GL/glut.h>     // 또는 <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#define RectSize 30
#define WindowXSize 800
#define WindowYSize 600

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);
void TimerFunction(int value);
void MenuFunc(int button);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
static int count = 0;
static int shape = 0;
static bool open = false;
bool check = false;
bool recheck = false;
struct rect {
	int x;
	int y;
	int size;
	float r;
	float g;
	float b;
	int speed;
	bool rBool = false;
	bool gBool = false;
	bool bBool = false;
	bool xBool = false;
	bool yBool = false;
	bool sizeUpdown = false;
	int count = 0;
	bool circle = false;
};

struct triangle {
	int x;
	int y;
	int size;
	int speed;
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
	bool circle = false;
};

static int tempX;
static int tempY;

static rect rectangle[10];
struct triangle tri[10];
static int total;
static int dir = 0;
static bool on = true;
void main(int argc, char** argv)    // 윈도우 출력하고 출력함수 설정
{   // 초기화 함수들 
	glutInit(&argc, argv);    // glut 초기화        
	int SubMenu1; 
	int MainMenu;
	SubMenu1 = glutCreateMenu(MenuFunc); 
	glutAddMenuEntry("Rectangle", 1); 
	glutAddMenuEntry("Triangle", 2);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  // 디스플레이 모드 설정        
	glutInitWindowPosition(100, 100);   // 윈도우의 위치 지정        
	glutInitWindowSize(800, 600);    // 윈도우의 크기 지정       
	glutCreateWindow("오예");   // 윈도우 생성 (윈도우 이름)         
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);    // 출력 함수의 지정         
	glutReshapeFunc(reshape);   // 다시 그리기 함수 지정     
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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

	if (shape == 0) {
		for (int i = 0; i < total; ++i) {
			glColor4f((float)rectangle[i].r / 255, (float)rectangle[i].g / 255, (float)rectangle[i].b / 255, 1.0f);
			glBegin(GL_POLYGON);
			glVertex2d(rectangle[i].x - rectangle[i].size, rectangle[i].y - rectangle[i].size);
			glVertex2d(rectangle[i].x - rectangle[i].size, rectangle[i].y + rectangle[i].size);
			glVertex2d(rectangle[i].x + rectangle[i].size, rectangle[i].y + rectangle[i].size);
			glVertex2d(rectangle[i].x + rectangle[i].size, rectangle[i].y - rectangle[i].size);
			glEnd();
		}
	}
	else if (shape == 1) {
		for (int i = 0; i < total; ++i) {
			glColor4f((float)tri[i].r / 255, (float)tri[i].g / 255, (float)tri[i].b / 255, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex2d(tri[i].x - tri[i].size, tri[i].y - tri[i].size);
			glVertex2d(tri[i].x, tri[i].y + tri[i].size);
			glVertex2d(tri[i].x + tri[i].size, tri[i].y - tri[i].size);
			glEnd();
			
		}
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

	if(shape == 0){
		for (int i = 0; i < total; ++i) {
			if (rectangle[i].xBool == false && rectangle[i].circle == false)
				rectangle[i].x -= rectangle[i].speed;
			else if (rectangle[i].xBool == true && rectangle[i].circle == false)
				rectangle[i].x += rectangle[i].speed;

			if (rectangle[i].yBool == false && rectangle[i].circle == false)
				rectangle[i].y -= rectangle[i].speed;
			else if (rectangle[i].yBool == true && rectangle[i].circle == false)
				rectangle[i].y += rectangle[i].speed;

			if (rectangle[i].x - rectangle[i].size - rectangle[i].speed < 0 && rectangle[i].circle == false)
				rectangle[i].xBool = true;
			else if (rectangle[i].x + rectangle[i].size + rectangle[i].speed > 800 && rectangle[i].circle == false)
				rectangle[i].xBool = false;

			if (rectangle[i].y - rectangle[i].size - rectangle[i].speed < 0 && rectangle[i].circle == false)
				rectangle[i].yBool = true;
			else if (rectangle[i].y + rectangle[i].size + rectangle[i].speed > 600 && rectangle[i].circle == false)
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
			if (rectangle[i].circle == true) {
				if (dir == 0) {
					rectangle[i].x += 1;
					if (rectangle[i].x + rectangle[i].size + 1 > 800)
						dir = 1;
					if (rectangle[i].x == tempX && rectangle[i].y == tempY) {
						rectangle[i].circle = false;
						recheck = false;
					}
				}
				else if (dir == 1) {
					rectangle[i].y -= 1;
					if (rectangle[i].y - rectangle[i].size - 1 < 0)
						dir = 2;
				}
				else if (dir == 2) {
					rectangle[i].x -= 1;
					if (rectangle[i].x - rectangle[i].size - 1 < 0)
						dir = 3;
				}
				else if (dir == 3) {
					rectangle[i].y += 1;
					if (rectangle[i].y == tempY)
						dir = 0;
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
	}
	else if (shape == 1) {
		for (int i = 0; i < total; ++i) {
			if (tri[i].xBool == false && tri[i].circle == false)
				tri[i].x -= tri[i].speed;
			else if (tri[i].xBool == true && tri[i].circle == false)
				tri[i].x += tri[i].speed;

			if (tri[i].yBool == false && tri[i].circle == false)
				tri[i].y -= tri[i].speed;
			else if (tri[i].yBool == true && tri[i].circle == false)
				tri[i].y += tri[i].speed;

			if (tri[i].x - tri[i].size - tri[i].speed < 0 && tri[i].circle == false)
				tri[i].xBool = true;
			else if (tri[i].x + tri[i].size + tri[i].speed > 800 && tri[i].circle == false)
				tri[i].xBool = false;

			if (tri[i].y - tri[i].size - tri[i].speed < 0 && tri[i].circle == false)
				tri[i].yBool = true;
			else if (tri[i].y + tri[i].size + tri[i].speed > 600 && tri[i].circle == false)
				tri[i].yBool = false;

			if (tri[i].sizeUpdown == false) {
				tri[i].count++;
				tri[i].size += 1;
				if (tri[i].count == 10) {
					tri[i].count = 0;
					tri[i].sizeUpdown = true;
				}
			}
			else if (tri[i].sizeUpdown == true) {
				tri[i].count++;
				tri[i].size -= 1;
				if (tri[i].count == 10) {
					tri[i].count = 0;
					tri[i].sizeUpdown = false;
				}
			}

			if (tri[i].circle == true) {
				if (dir == 0) {
					tri[i].x += 1;
					if (tri[i].x + tri[i].size + 1 > 800)
						dir = 1;
					if (tri[i].x == tempX && tri[i].y == tempY) {
						tri[i].circle = false;
						recheck = false;
					}
				}
				else if (dir == 1) {
					tri[i].y -= 1;
					if (tri[i].y - tri[i].size - 1 < 0)
						dir = 2;
				}
				else if (dir == 2) {
					tri[i].x -= 1;
					if (tri[i].x - tri[i].size - 1 < 0)
						dir = 3;
				}
				else if (dir == 3) {
					tri[i].y += 1;
					if (tri[i].y == tempY)
						dir = 0;
				}
			}

			if (tri[i].rBool == false) {
				tri[i].r += 1;
				if (tri[i].r == 255)
					tri[i].rBool = true;
			}
			else if (tri[i].rBool == true) {
				tri[i].r -= 1;
				if (tri[i].r == 0)
					tri[i].rBool = false;
			}

			if (tri[i].gBool == false) {
				tri[i].g += 1;
				if (tri[i].g == 255)
					tri[i].gBool = true;
			}
			else if (tri[i].gBool == true) {
				tri[i].g -= 1;
				if (tri[i].g == 0)
					tri[i].gBool = false;
			}

			if (tri[i].bBool == false) {
				tri[i].b += 1;
				if (tri[i].b == 255)
					tri[i].bBool = true;
			}
			else if (tri[i].bBool == true) {
				tri[i].r -= 1;
				if (tri[i].b == 0)
					tri[i].bBool = false;
			}
		}

	}

	if(on == true)
		glutTimerFunc(10, TimerFunction, 1); // 타이머함수 재 설정
}

void Mouse(int button, int state, int x, int y)
{
	int tempX;
	int tempY;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (shape == 0) {
			rectangle[count].x = x;
			rectangle[count].y = y;
			rectangle[count].size = rand() % 40 + 20;
			rectangle[count].r = rand() % 255;
			rectangle[count].g = rand() % 255;
			rectangle[count].b = rand() % 255;
			rectangle[count].speed = rand() % 10 + 1;
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
		}
		else if (shape == 1) {
			tri[count].x = x;
			tri[count].y = y;
			tri[count].size = rand() % 40 + 20;
			tri[count].r = rand() % 255;
			tri[count].g = rand() % 255;
			tri[count].b = rand() % 255;
			tri[count].speed = rand() % 10 + 1;
			tempX = rand() % 2;
			tempY = rand() % 2;

			if (tempX == 0)
				tri[count].xBool = false;
			else if (tempX == 1)
				tri[count].xBool = true;

			if (tempY == 0)
				tri[count].yBool = false;
			else if (tempY == 1)
				tri[count].yBool = true;
		}

		count++;
		if (check == false)
			total = count;
		if (count >= 10) {
			count %= 10;
			total = 10;
			check = true;
		}
	}

}

void MenuFunc(int button) 
{
	switch (button) { 
		case 1:
			shape = 0;
			count = 0;
			check = false;
			break; 
		case 2:
			shape = 1;
			count = 0;
			check = false;
			break;
		case 11:
			break;

	} 
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		on = false;
		break;
	case 'p':
		on = true;
		glutTimerFunc(10, TimerFunction, 1);
		break;

	case 'u':
		for (int i = 0; i < 10; ++i) {
			tri[i].speed += 1;
			rectangle[i].speed += 1;
		}
		break;

	case 'i':
		for (int i = 0; i < 10; ++i) {
			tri[i].speed -= 1;
			rectangle[i].speed -= 1;
		}
		break;

	case '1':
		if (recheck == false) {
			if (shape == 1) {
				tri[1].circle = true;
				tempX = tri[1].x;
				tempY = tri[1].y;
			}
			else if (shape == 0) {
				rectangle[1].circle = true;
				tempX = rectangle[1].x;
				tempY = rectangle[1].y;
			}
			recheck = true;
		}
		break;

	case '2':
		if (recheck == false) {
			if (shape == 1) {
				tri[2].circle = true;
				tempX = tri[2].x;
				tempY = tri[2].y;
			}
			else if (shape == 0) {
				rectangle[2].circle = true;
				tempX = rectangle[2].x;
				tempY = rectangle[2].y;
			}
			recheck = true;
		}
		break;

	case '3':
		if (recheck == false) {
			if (shape == 1) {
				tri[3].circle = true;
				tempX = tri[3].x;
				tempY = tri[3].y;
			}
			else if (shape == 0) {
				rectangle[3].circle = true;
				tempX = rectangle[3].x;
				tempY = rectangle[3].y;
			}
			recheck = true;
		}
		break;

	case '4':
		if (recheck == false) {
			if (shape == 1) {
				tri[4].circle = true;
				tempX = tri[4].x;
				tempY = tri[4].y;
			}
			else if (shape == 0) {
				rectangle[4].circle = true;
				tempX = rectangle[4].x;
				tempY = rectangle[4].y;
			}
			recheck = true;
		}
		break;

	case '5':
		if (recheck == false) {
			if (shape == 1) {
				tri[5].circle = true;
				tempX = tri[5].x;
				tempY = tri[5].y;
			}
			else if (shape == 0) {
				rectangle[5].circle = true;
				tempX = rectangle[5].x;
				tempY = rectangle[5].y;
			}
			recheck = true;
		}
		break;

	case '6':
		if (recheck == false) {
			if (shape == 1) {
				tri[6].circle = true;
				tempX = tri[6].x;
				tempY = tri[6].y;
			}
			else if (shape == 0) {
				rectangle[6].circle = true;
				tempX = rectangle[6].x;
				tempY = rectangle[6].y;
			}
			recheck = true;
		}
		break;

	case '7':
		if (recheck == false) {
			if (shape == 1) {
				tri[7].circle = true;
				tempX = tri[7].x;
				tempY = tri[7].y;
			}
			else if (shape == 0) {
				rectangle[7].circle = true;
				tempX = rectangle[7].x;
				tempY = rectangle[7].y;
			}
			recheck = true;
		}
		break;

	case '8':
		if (recheck == false) {
			if (shape == 1) {
				tri[8].circle = true;
				tempX = tri[8].x;
				tempY = tri[8].y;
			}
			else if (shape == 0) {
				rectangle[8].circle = true;
				tempX = rectangle[8].x;
				tempY = rectangle[8].y;
			}
			recheck = true;
		}
		break;

	case '9':
		if (recheck == false) {
			if (shape == 1) {
				tri[9].circle = true;
				tempX = tri[9].x;
				tempY = tri[9].y;
			}
			else if (shape == 0) {
				rectangle[9].circle = true;
				tempX = rectangle[9].x;
				tempY = rectangle[9].y;
			}
			recheck = true;
		}
		break;

	case '0':
		if (recheck == false) {
			if (shape == 1) {
				tri[0].circle = true;
				tempX = tri[0].x;
				tempY = tri[0].y;
			}
			else if (shape == 0) {
				rectangle[0].circle = true;
				tempX = rectangle[0].x;
				tempY = rectangle[0].y;
			}
			recheck = true;
		}
		break;
	}

}
