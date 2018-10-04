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
static BOOL to = TRUE;
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
			if (to == TRUE)
				glRotated(rad, 0, 0, 1);
			glTranslated((rectleftX - size) * -1, (rectleftY - size) * -1, 0);
		}
		else if (circleShape == 2) {
			glTranslated(triCenterX, triCenterY, 0);
			if(to == TRUE)
				glRotated(rad, 0, 0, 1);
			glTranslated(triCenterX * -1, triCenterY * -1, 0);
		}
	}
	glColor4f((float)255, (float)0, 0.0f, 1.0f);

	if(circleShape == 1 && shape != 4)
		glRectf(rectleftX, rectleftY, rectrightX, rectrightY);
	else if (circleShape == 1 && shape == 4) {
		glRectf(rectleftX + size, rectleftY + size, rectrightX - size, rectrightY - size);
	}
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

	case 'k':
		if (to == FALSE)
			to = TRUE;
		else
			to = FALSE;
		break;
	}
}


static BOOL check = FALSE;
void TimerFunction(int value)
{
	glutPostRedisplay();
	
	if (shape == 1) {
		rad += 10;
		if (circleShape == 1 && shape == 1) {
			rectleftX = ci[0].circleVertex[count].x + size;
			rectleftY = ci[0].circleVertex[count].y + size;
			rectrightX = ci[0].circleVertex[count].x - size;
			rectrightY = ci[0].circleVertex[count].y - size;
		}
		//else if (circleShape == 1 && shape != 1) {
		//	rectleftX = ci[0].circleVertex[count].x;
		//	rectleftY = ci[0].circleVertex[count].y;
		//	rectrightX = ci[0].circleVertex[count].x;
		//	rectrightY = ci[0].circleVertex[count].y;
		//}
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
			rectleftX = rect[count].x + size;
			rectleftY = rect[count].y + size;
			rectrightX = rect[count].x - size;
			rectrightY = rect[count].y - size;
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
			rectleftX = ci[0].circleVertex[count].x + size;
			rectleftY = ci[0].circleVertex[count].y + size;
			rectrightX = ci[0].circleVertex[count].x - size;
			rectrightY = ci[0].circleVertex[count].y - size;
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

		if (count == 8) {
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

//#include <GL/freeglut.h> 
//#include <random>
//#include <time.h>
//#include <math.h>
//
//GLvoid Reshape(int w, int h);
//
////해상도 설정
//#define WideSize 800
//#define HighSize 600
//#define Z_Size 400
//
////그리기 제어
//GLvoid drawScene(GLvoid);
//BOOL draw_Act_On;
//
////타이머 제어
//void Timerfunction(int value);
//BOOL Time_Act_On; //타이머 활성화 여부
//int Time_count; //타이머 카운트
//
//				//마우스 제어
//void Mouse(int button, int state, int x, int y);
//POINT ms_click; //마우스 좌표
//BOOL Mouse_Act_On; //마우스 활성화 여부
//
//				   //키보드 제어
//void Keyboard(unsigned char key, int x, int y);
//BOOL Keyboard_Act_On; //키보드 활성화 여부
//
//					  //색 선정
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
//#define PT 100//도형 갯수 설정
//#define PI 3.141592 //파이
//typedef struct Shape
//{
//	Color cl;//색상
//	Translate_pos pos;
//	Translate_pos move;
//	Translate_pos scale;
//	Translate_pos rot;
//	Translate_pos sa;
//	int size;
//	int select;
//	int height;
//	int slice;
//	int stacks;
//};
//struct PO {
//	float x;
//	float y;
//};
//int click_count;
//int save_count;
//int st_help;//직선 그리기 도우미
//
//int zig_zag;//지그재그 그리기 도우미
//BOOL ZIG;//지그재그
//
//BOOL Save = false;
//BOOL ani = FALSE;
//BOOL Look = FALSE;
//
//Shape sh[PT];
//PO save;
//
//
//
//void main(int argc, char *argv[]) {
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(100, 100);//윈도우 띄우기 좌표
//	glutInitWindowSize(WideSize, HighSize); //윈도우 띄우기 크기
//	glutCreateWindow("ex11");
//	// - 랜덤으로 시작 도형 설정하기
//	//도형 그리기
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
//	glPointSize(4);
//	glLineWidth(2);
//	glColor3f((float)255 / 255, (float)255 / 255, (float)255 / 255);
//
//
//	glMatrixMode(GL_MODELVIEW);
//	//출력 설정
//	glColor3f((float)100 / 255, (float)200 / 255, (float)100 / 255);
//	//좌표축 그리기
//
//	glPushMatrix(); //상태 저장 열기
//	if (!Mouse_Act_On) {
//		glTranslated(WideSize / 2, HighSize / 2, 0);
//	}
//
//	for (int i = 0; i <  PT - 1; i++) {
//		glBegin(GL_LINES);
//		glVertex3d(sh[i].pos.x, sh[i].pos.y, sh[i].pos.z);
//		glVertex3d(sh[i + 1].pos.x, sh[i + 1].pos.y, sh[i + 1].pos.z);
//		glEnd();
//	}
//	glPopMatrix(); //상태 저장 닫기
//
//
//	glutSwapBuffers();
//}
//
//void Mouse(int button, int state, int x, int y) {
//
//	if (Mouse_Act_On) {
//		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//		{
//			if (click_count < 5) {
//				sh[click_count].sa.x = x;
//				sh[click_count].sa.y = y;
//			}
//			click_count++;
//			save_count++;
//		}
//		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {//우클릭시 그리기 시작
//			Save = TRUE;
//			click_count = 0;
//			for (int i = 0; i < PT; i++) {
//				if (i % (PT / (save_count - 1)) == 0) {
//
//					sh[i].pos.x = sh[click_count].sa.x;
//					sh[i].pos.y = sh[click_count].sa.y;
//
//					save.x = (float)(sh[click_count + 1].sa.x - sh[click_count].sa.x) / (PT / (save_count - 1));
//					save.y = (float)(sh[click_count + 1].sa.y - sh[click_count].sa.y) / (PT / (save_count - 1));
//					if (click_count != save_count - 2) {
//						click_count++;
//					}
//				}
//				else {
//						sh[i].pos.x = save.x + sh[i - 1].pos.x;
//						sh[i].pos.y = save.y + sh[i - 1].pos.y;
//				}
//			}
//		}
//	}
//}
//
//void Timerfunction(int value) {
//	Time_count++;
//	//타이머 내용 입력
//
//
//	glutPostRedisplay(); //타이머에 넣는다.
//	glutTimerFunc(100, Timerfunction, 1); //타이머 다시 출력
//
//}
//
//void Keyboard(unsigned char key, int x, int y) {
//	switch (key)
//	{
//	case '0'://원
//		Mouse_Act_On = FALSE;
//		for (int i = 0; i < PT; i++) {
//			sh[i].pos.x = 100 * cos(PI * i * 10 / 180);
//			sh[i].pos.y = 100 * sin(PI * i * 10 / 180);
//		}
//		break;
//
//	case '1'://사인
//		Mouse_Act_On = FALSE;
//		for (int i = 0; i < PT; i++) {
//			sh[i].pos.x = i * 10 - WideSize / 2;
//			sh[i].pos.y = 100 * sin(PI * i * 10 / 90);
//		}
//		break;
//	case '2'://회오리
//		Mouse_Act_On = FALSE;
//		for (int i = 0; i < PT; i++) {
//			sh[i].pos.x = i * 10 + 100 * cos(PI * i * 10 / 180) - WideSize / 2;
//			sh[i].pos.y = 100 * sin(PI * i * 10 / 180);
//		}
//		break;
//	case '3'://지그재그
//		st_help = 0;
//		zig_zag = 1;
//		Save = FALSE;
//		Mouse_Act_On = FALSE;
//		for (int i = 0; i < PT; i++) {
//
//			if (i % 20 == 0) {
//				zig_zag *= (-1);
//				//지그재그로 교차되는 선을 만든다.
//			}
//
//			if (ZIG) {
//				st_help--;
//			}
//			else {
//				st_help++;
//			}
//
//			sh[i].pos.x = i * 10 - WideSize / 2;
//			sh[i].pos.y = zig_zag * (st_help * 5);
//
//			if (st_help == 10) {
//				ZIG = TRUE;
//			}
//			else if (st_help == -10) {
//				ZIG = FALSE;
//			}
//		}
//		break;
//	case '4'://경로 그리기
//		st_help = 0;//직선 그리기 도우미
//		Mouse_Act_On = TRUE;//우클릭을 누르지 않으면 하지 않는다.
//		click_count = 0;
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
//	glOrtho(0, WideSize, HighSize, 0, -Z_Size / 2, Z_Size / 2); //윈도우를 초기화 하는 함수입니다!
//
//}