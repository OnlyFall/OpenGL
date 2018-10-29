#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

static int mode = 1;

#define pi 3.14
#define DEGTORAD pi/360 * 2
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15


struct point { float x, y; };

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define Ax (pA->x)
#define Ay (pA->y)
#define Bx (pB->x)
#define By (pB->y)
#define Cx (pC->x)
#define Cy (pC->y)
#define Dx (pD->x)
#define Dy (pD->y)

int check_intersect(struct point * pA, struct point * pB, struct point * pC, struct point * pD);

struct point_3 {
	float x = 0;
	float y = 0;
	float z = 60;
};

static point_3 spherePos;

typedef struct set {
	float x;
	float y;
	float z;
};

typedef struct Translate_pos {
	float x;
	float y;
	float z;
	float degree;
};

typedef struct Shape
{
	Translate_pos pos;
	Translate_pos move;
	Translate_pos scale;
	Translate_pos rot;

	int size;
	int select;
	int height;
	int slice;
	int stacks;

	BOOL any;

};

Translate_pos EYE;
Translate_pos AT;
Translate_pos UP;



struct DOUBLE_3D_POS
{
	double x, y, z;
};

class Cam
{
private:
	DOUBLE_3D_POS m_Eye = {};
	DOUBLE_3D_POS m_Center = {};
	DOUBLE_3D_POS m_Up = {};
	DOUBLE_3D_POS m_MoveEye = {};
	DOUBLE_3D_POS m_Angle = {};

public:
	Cam() // 생성자
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}
	void Init() // 초기화 함수
	{
		m_Eye = { 0.0, 0.0, 0.0 };
		m_Center = { 0.0, 0.0, -100.0 };
		m_Up = { 0.0, 1.0, 0.0 };
		m_MoveEye = { 0.0, 0.0, 0.0 };
		m_Angle = { 0.0, 0.0, 0.0 };
	}

	void drawCamera() // 카메라를 배치하는 함수
	{
		//glPushMatrix();
		//glLoadIdentity(); // 현재의 변환 행렬을 단위행렬로 초기화 - 시각 좌표계를 원점으로 초기화
		// 관측 변환: 카메라의 위치 설정 (필요한 경우, 다른 곳에 설정 가능)
		gluLookAt(m_Eye.x, m_Eye.y, m_Eye.z, m_Center.x, m_Center.y, m_Center.z, m_Up.x, m_Up.y, m_Up.z);

		glRotated(-m_Angle.x, 0.0, 1.0, 0.0);
		glRotated(m_Angle.y, 1.0, 0.0, 0.0);
		glRotated(m_Angle.z, 0.0, 0.0, 1.0);
		glTranslated(-m_MoveEye.x, -m_MoveEye.y, m_MoveEye.z);
		//glPopMatrix();
	}

public:
	void rotateEye(double x, double y, double z) // 카메라 회전 함수
	{
		m_Angle.x += x;
		m_Angle.y += y;
		m_Angle.z += z;
	}
	void moveEye(double x, double y, double z) // 카메라 이동 함수
	{
		m_MoveEye.x += x;
		m_MoveEye.y += y;
		m_MoveEye.z += z;
	}
};

struct STAR {
	float x;
	float y;

	int rad;
	float size = 1;

	int destinationX;
	int destinationY;

	float moveX;
	float moveY;

	int R;
	int G;
	int B;
	float r = 20;
	BOOL updown = FALSE;
	int count;
};



struct movingTriangle {
	int x;
	int y;

	int rad = 0;
	BOOL visible = TRUE;
	BOOL check = FALSE;
};

struct star {
	float x;
	float y;

	BOOL check = FALSE;
};

struct savePiece {
	int x;
	int y;
	BOOL leftPiece = FALSE;
	BOOL RightPiece = FALSE;

	
};

struct Rect {
	float x;
	float y;

	int slice = 0;


};

struct SliceTri {
	float x;
	float y;

	int destinationX;
	int destinationY;

	float xMove;
	float yMove;

	int count = 0;

	float rad = 0;
	float radMove = 0;

	int sliceDir = 0;
	BOOL click = FALSE;
	BOOL end = FALSE;
	BOOL SaveOn;
};

struct mouseLine {
	int startX;
	int startY;
	int endX;
	int endY;
};

static BOOL left = FALSE;

static mouseLine ML;

static SliceTri SliceTriangle[2];

static movingTriangle Tri[8];
static Rect rect;

static savePiece Leftsave[7][3];
static savePiece Rightsave[7][3];

static Cam camera;

static movingTriangle saveLeftTri[42];
static movingTriangle saveRightTri[42];

static STAR mystar[100];

static int BlineNumber = 2;
static int starcount = 0;
static int saveCount = 0;
static int saveCount2 = 0;

void SetupRC()
{
	camera.Init();
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 3; ++j) {
			Leftsave[i][j].x = i * 50 + 25;
			Leftsave[i][j].y = j * 50 + 25;

			Rightsave[i][j].x = 800 - (i * 50 + 25);
			Rightsave[i][j].y = j * 50 + 25;
		}
	}

	for (int i = 0; i < 8; ++i) {
		Tri[i].x = i * 100 + 50;
		Tri[i].y = 550;
		Tri[i].rad = rand() % 360;
	}

	SliceTriangle[0].SaveOn = TRUE;
	SliceTriangle[1].SaveOn = TRUE;
	rect.x = 400;
	rect.y = -50;
}
void processMousePassiveMotion(int x, int y);
static BOOL slice = FALSE;
void main(int agrc, char *argv[]) { // 윈도우 초기화 및 생성 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// 더블 버퍼링과 3차원으로 그린다. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// 상태 변수 초기화 함수 
	SetupRC();
	// 필요한 콜백 함수 설정
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(processMousePassiveMotion);
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수
	glutMainLoop(); // 이벤트 루프 실행하기
}

GLvoid DrawLine(GLvoid)
{
	for (int i = 0; i < 3; ++i) {
		glBegin(GL_LINES);
		glVertex2f(0, (i + 1) * 50);
		glVertex2f(350, (i + 1) * 50);
		glEnd();
	}

	for (int i = 0; i < 7; ++i) {
		glBegin(GL_LINES);
		glVertex2f((i + 1) * 50, 0);
		glVertex2f((i + 1) * 50, 150);
		glEnd();
	}

	for (int i = 0; i < 3; ++i) {
		glBegin(GL_LINES);
		glVertex2f(450, (i + 1) * 50);
		glVertex2f(800, (i + 1) * 50);
		glEnd();
	}

	for (int i = 0; i < 7; ++i) {
		glBegin(GL_LINES);
		glVertex2f((i + 1) * 50 + 400, 0);
		glVertex2f((i + 1) * 50 + 400, 150);
		glEnd();
	}

	glBegin(GL_LINES);
	glVertex2f(0, 500);
	glVertex2f(800, 500);
	glEnd();
}

GLvoid DrawTriangle(GLvoid)
{
	for (int i = 0; i < 8; ++i) {
		glPushMatrix();
		if (Tri[i].visible == TRUE) {
			glTranslatef(Tri[i].x, Tri[i].y, 0);
			glRotatef(Tri[i].rad, 0, 0, 1);
			glTranslatef(-Tri[i].x, -Tri[i].y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2f(Tri[i].x - 25, Tri[i].y - 25);
			glVertex2f(Tri[i].x + 25, Tri[i].y - 25);
			glVertex2f(Tri[i].x, Tri[i].y + 25);
			glEnd();
			glPopMatrix();
		}
	}
}

GLvoid DrawRectangle()
{
	if (rect.slice == 0) {
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
		glVertex2f(rect.x - 25, rect.y - 25);
		glVertex2f(rect.x - 25, rect.y + 25);
		glVertex2f(rect.x + 25, rect.y + 25);
		glVertex2f(rect.x + 25, rect.y - 25);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
	}

	else if (rect.slice == 1) {

		if (BlineNumber != 0) {
			glColor3f(1.0f, 1.0f, 0.0f);
			glPushMatrix();
			glTranslatef(SliceTriangle[0].x, SliceTriangle[0].y, 0);
			glRotatef(-SliceTriangle[0].rad, 0, 0, 1);
			glTranslatef(-SliceTriangle[0].x, -SliceTriangle[0].y, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(SliceTriangle[0].x, SliceTriangle[0].y + 50);
			glVertex2f(SliceTriangle[0].x, SliceTriangle[0].y);
			glVertex2f(SliceTriangle[0].x + 50, SliceTriangle[0].y);
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnd();
			glPopMatrix();
		}
		
		if (BlineNumber != 1) {
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f);
			glTranslatef(SliceTriangle[1].x, SliceTriangle[1].y, 0);
			glRotatef(-SliceTriangle[1].rad, 0, 0, 1);
			glTranslatef(-SliceTriangle[1].x, -SliceTriangle[1].y, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(SliceTriangle[1].x - 50, SliceTriangle[1].y);
			glVertex2f(SliceTriangle[1].x, SliceTriangle[1].y);
			glVertex2f(SliceTriangle[1].x, SliceTriangle[1].y - 50);
			glEnd();
			glColor3f(1.0f, 1.0f, 1.0f);
			glPopMatrix();
		}
	}

	else if (rect.slice == 2) {
		if (BlineNumber != 0) {
			glColor3f(1.0f, 1.0f, 0.0f);
			glPushMatrix();
			glTranslatef(SliceTriangle[0].x, SliceTriangle[0].y, 0);
			glRotatef(-SliceTriangle[0].rad, 0, 0, 1);
			glTranslatef(-SliceTriangle[0].x, -SliceTriangle[0].y, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(SliceTriangle[0].x, SliceTriangle[0].y);
			glVertex2f(SliceTriangle[0].x + 50, SliceTriangle[0].y);
			glVertex2f(SliceTriangle[0].x, SliceTriangle[0].y - 50);
			glEnd();
			glPopMatrix();
		}

		if (BlineNumber != 1) {
			glPushMatrix();
			glColor3f(1.0f, 1.0f, 0.0f);
			glTranslatef(SliceTriangle[1].x, SliceTriangle[1].y, 0);
			glRotatef(-SliceTriangle[1].rad, 0, 0, 1);
			glTranslatef(-SliceTriangle[1].x, -SliceTriangle[1].y, 0);
			glBegin(GL_TRIANGLES);
			glVertex2f(SliceTriangle[1].x, SliceTriangle[1].y);
			glVertex2f(SliceTriangle[1].x, SliceTriangle[1].y + 50);
			glVertex2f(SliceTriangle[1].x - 50, SliceTriangle[1].y);
			glEnd();
			glPopMatrix();
			glColor3f(1.0f, 1.0f, 1.0f);
		}
	}
}

GLvoid DrawML(GLvoid)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(ML.startX, ML.startY);
	glVertex2f(ML.endX, ML.endY);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}

GLvoid StarLight(GLvoid)
{
	for (int i = 0; i < starcount; ++i) {
		glPushMatrix();
		glTranslatef(mystar[i].x, mystar[i].y, 0);
		glScalef(mystar[i].size, mystar[i].size, 1);
		glRotatef(mystar[i].rad, 0, 0, 1);
		glTranslatef(-mystar[i].x, -mystar[i].y, 0);
		glColor3f((float)mystar[i].R / 255, (float)mystar[i].G / 255, (float)mystar[i].B / 255);
		glBegin(GL_TRIANGLES);
		glVertex2f(mystar[i].x - mystar[i].r, mystar[i].y - mystar[i].r);
		glVertex2f(mystar[i].x + mystar[i].r, mystar[i].y - mystar[i].r);
		glVertex2f(mystar[i].x, mystar[i].y + mystar[i].r);
		glEnd();

		glPushMatrix();
		glTranslatef(0, - 15, 0);
		glTranslatef(mystar[i].x, mystar[i].y - 5, 0);
		glRotatef(180, 1, 0, 0);
		glTranslatef(-mystar[i].x, -(mystar[i].y), 0);

		glBegin(GL_TRIANGLES);
		glVertex2f(mystar[i].x - mystar[i].r, mystar[i].y - mystar[i].r);
		glVertex2f(mystar[i].x + mystar[i].r, mystar[i].y - mystar[i].r);
		glVertex2f(mystar[i].x, mystar[i].y + mystar[i].r);
		glEnd();

		glPopMatrix();
		glPopMatrix();
		
	}
	glColor3f(1.0f, 1.0f, 1.0f);
}

GLvoid DrawSave(GLvoid)
{


	for (int i = 0; i < saveCount; ++i) {
		glBegin(GL_TRIANGLES);
		glVertex2f(saveRightTri[i].x, saveRightTri[i].y);
		glVertex2f(saveRightTri[i].x - 50, saveRightTri[i].y);
		glVertex2f(saveRightTri[i].x, saveRightTri[i].y - 50);
		glEnd();
	}

	for (int j = 0; j < saveCount2; ++j) {
		glBegin(GL_TRIANGLES);
		glVertex2f(saveLeftTri[j].x, saveLeftTri[j].y);
		glVertex2f(saveLeftTri[j].x + 50, saveLeftTri[j].y);
		glVertex2f(saveLeftTri[j].x, saveLeftTri[j].y + 50);
		glEnd();
	}
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	
	DrawLine();
	DrawTriangle();
	DrawRectangle();
	DrawSave();
	
	if(left == true)
		DrawML();
	
	glPushMatrix();
	StarLight();
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
	//	glFlush(); // 화면에 출력하기 
}

static BOOL starCheck = FALSE;
static BOOL clip = FALSE;
static float zZoom = 45;
BOOL starCrash = FALSE;
GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(0, 0, w, h);  // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	bool transe = false;
	//-- 투영은 직각 투영 또는 원근 투영 중 한 개를 설정한다. // 1. 클리핑 공간 설정: 원근 투영인 경우
	if (transe) //원근투영
	{
		gluPerspective(45.0f, 1.0, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else //직각투영
		glOrtho(0, 800, 0, 600, -400, 400);

	//모델 뷰 행렬 스택 재설정
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TimerFunction(int value)
{
	glutPostRedisplay();

	for (int i = 0; i < 8; ++i) {
		Tri[i].x += 1;
		Tri[i].rad = (Tri[i].rad + 1) % 360;
		if (Tri[i].x > 825) {
			Tri[i].x = -25;
			Tri[i].visible = TRUE;
		}
	}

	if (rect.slice == 0) {
		rect.y += 1;
		if (rect.y > 625)
			rect.y = -25;
	}

	else if (rect.slice == 1) {
		if (SliceTriangle[0].count < 100) {
			if (SliceTriangle[0].click == FALSE) {
				SliceTriangle[0].x += SliceTriangle[0].xMove;
				SliceTriangle[0].y += SliceTriangle[0].yMove;
				SliceTriangle[0].rad += SliceTriangle[0].radMove;
				SliceTriangle[0].count++;
			}
		}

		if (SliceTriangle[1].count < 100) {
			if (SliceTriangle[1].click == FALSE) {
				SliceTriangle[1].x += SliceTriangle[1].xMove;
				SliceTriangle[1].y += SliceTriangle[1].yMove;
				SliceTriangle[1].rad += SliceTriangle[1].radMove;
				SliceTriangle[1].count++;
			}
		}

		if (SliceTriangle[0].click == TRUE && starCheck == TRUE) {
			if (mystar[starcount - 1].count < 100) {
				mystar[starcount - 1].x += mystar[starcount - 1].moveX;
				mystar[starcount - 1].y += mystar[starcount - 1].moveY;
				mystar[starcount - 1].count++;

				for (int i = 0; i < starcount - 2; ++i) {
					if (mystar[starcount - 1].r + mystar[i].r > sqrtf((mystar[starcount - 1].x - mystar[i].x) * (mystar[starcount - 1].x - mystar[i].x) + (mystar[starcount - 1].y - mystar[i].y) * (mystar[starcount - 1].y - mystar[i].y))) {
						mystar[starcount - 1].count = 100;
						mystar[i].r += 5;
						starCrash = TRUE;
					}
				}
			}
			else if (mystar[starcount - 1].count == 100) {
				SliceTriangle[0].SaveOn = FALSE;
				SliceTriangle[0].end = FALSE;
				SliceTriangle[0].click = FALSE;
				SliceTriangle[0].count = 100;

				if (starCrash == TRUE) {
					mystar[starcount - 1].count = 0;
					starcount -= 1;
				}
				starCheck = FALSE;
			}
		}

		if (SliceTriangle[1].click == TRUE && starCheck == TRUE) {
			if (mystar[starcount - 1].count < 100) {
				mystar[starcount - 1].x += mystar[starcount - 1].moveX;
				mystar[starcount - 1].y += mystar[starcount - 1].moveY;
				mystar[starcount - 1].count++;

				for (int i = 0; i < starcount - 2; ++i) {
					if (mystar[starcount - 1].r + mystar[i].r > sqrtf((mystar[starcount - 1].x - mystar[i].x) * (mystar[starcount - 1].x - mystar[i].x) + (mystar[starcount - 1].y - mystar[i].y) * (mystar[starcount - 1].y - mystar[i].y))) {
						mystar[starcount - 1].count = 100;
						mystar[i].r += 5;
						starCrash = TRUE;
					}
				}
			}
			else if (mystar[starcount - 1].count == 100) {
				SliceTriangle[1].SaveOn = FALSE;
				SliceTriangle[1].end = FALSE;
				SliceTriangle[1].click = FALSE;
				SliceTriangle[1].count = 100;

				if (starCrash == TRUE) {
					mystar[starcount - 1].count = 0;
					starcount -= 1;
				}
				starCheck = FALSE;
			}
		}

		if (SliceTriangle[0].count == 100 && SliceTriangle[0].end == FALSE) {
			//	saveRightTri[saveCount].x = SliceTriangle[1].x;
			if (SliceTriangle[1].SaveOn == TRUE) {
				saveRightTri[saveCount].x = Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].x + 25;
				//	saveRightTri[saveCount].y = SliceTriangle[1].y;
				saveRightTri[saveCount].y = Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].y + 25;

				saveCount++;
			}
			SliceTriangle[0].end = TRUE;
		}

		if (SliceTriangle[1].count == 100 && SliceTriangle[1].end == FALSE) {
			//	saveLeftTri[saveCount].x = SliceTriangle[0].x;

			if (SliceTriangle[0].SaveOn == TRUE) {
				saveLeftTri[saveCount2].x = Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].x - 25;
				//	saveLeftTri[saveCount].y = SliceTriangle[0].y;
				saveLeftTri[saveCount2].y = Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].y - 25;
				saveCount2++;
			}
			SliceTriangle[1].end = TRUE;
		}

		if (SliceTriangle[0].count >= 100 && SliceTriangle[1].count >= 100) {
			SliceTriangle[0].rad = 0;
			SliceTriangle[1].rad = 0;
			rect.slice = 0;
			rect.y = -25;
			SliceTriangle[0].count = 0;
			SliceTriangle[1].count = 0;
			slice = FALSE;

			BlineNumber = 2;
			SliceTriangle[0].end = FALSE;
			SliceTriangle[1].end = FALSE;
			SliceTriangle[0].click = FALSE;
			SliceTriangle[1].click = FALSE;
			SliceTriangle[0].SaveOn = TRUE;
			SliceTriangle[1].SaveOn = TRUE;
		}
	}

	else if (rect.slice == 2) {

		if (SliceTriangle[0].count < 100) {
			if (SliceTriangle[0].click == FALSE) {
				SliceTriangle[0].x += SliceTriangle[0].xMove;
				SliceTriangle[0].y += SliceTriangle[0].yMove;
				SliceTriangle[0].rad += SliceTriangle[0].radMove;
				SliceTriangle[0].count++;
			}
		}
		
		if (SliceTriangle[1].count < 100) {
			if (SliceTriangle[1].click == FALSE) {
				SliceTriangle[1].x += SliceTriangle[1].xMove;
				SliceTriangle[1].y += SliceTriangle[1].yMove;
				SliceTriangle[1].rad += SliceTriangle[1].radMove;
				SliceTriangle[1].count++;
			}
		}

		if (SliceTriangle[0].click == TRUE && starCheck == TRUE) {
			if (mystar[starcount - 1].count < 100) {
				mystar[starcount - 1].x += mystar[starcount - 1].moveX;
				mystar[starcount - 1].y += mystar[starcount - 1].moveY;
				mystar[starcount - 1].count++;

				for (int i = 0; i < starcount - 2; ++i) {
					if (mystar[starcount - 1].r + mystar[i].r > sqrtf((mystar[starcount - 1].x - mystar[i].x) * (mystar[starcount - 1].x - mystar[i].x) + (mystar[starcount - 1].y - mystar[i].y) * (mystar[starcount - 1].y - mystar[i].y))) {
						mystar[starcount - 1].count = 100;
						mystar[i].r += 5;
						starCrash = TRUE;
					}
				}
			}
			else if (mystar[starcount - 1].count == 100) {
				SliceTriangle[0].SaveOn = FALSE;
				SliceTriangle[0].end = FALSE;
				SliceTriangle[0].click = FALSE;
				SliceTriangle[0].count = 100;

				if (starCrash == TRUE) {
					mystar[starcount - 1].count = 0;
					starcount -= 1;
				}

				starCheck = FALSE;
			}
		}

		if (SliceTriangle[1].click == TRUE && starCheck == TRUE) {
			if (mystar[starcount - 1].count < 100) {
				mystar[starcount - 1].x += mystar[starcount - 1].moveX;
				mystar[starcount - 1].y += mystar[starcount - 1].moveY;
				mystar[starcount - 1].count++;

				for (int i = 0; i < starcount - 2; ++i) {
					if (mystar[starcount - 1].r + mystar[i].r > sqrtf((mystar[starcount - 1].x - mystar[i].x) * (mystar[starcount - 1].x - mystar[i].x) + (mystar[starcount - 1].y - mystar[i].y) * (mystar[starcount - 1].y - mystar[i].y))) {
						mystar[starcount - 1].count = 100;
						mystar[i].r += 5;
						starCrash = TRUE;
					}
				}
			}
			
			else if (mystar[starcount - 1].count == 100) {
				SliceTriangle[1].SaveOn = FALSE;
				SliceTriangle[1].end = FALSE;
				SliceTriangle[1].click = FALSE;
				SliceTriangle[1].count = 100;
				if (starCrash == TRUE) {
					mystar[starcount - 1].count = 0;
					starcount -= 1;
				}
				starCheck = FALSE;
			}
		}

		if (SliceTriangle[0].count == 100 && SliceTriangle[0].end == FALSE) {
			if (SliceTriangle[0].SaveOn == TRUE) {

				//	saveLeftTri[saveCount].x = SliceTriangle[0].x;
				saveRightTri[saveCount].x = Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].x + 25;
				//	saveLeftTri[saveCount].y = SliceTriangle[0].y;
				saveRightTri[saveCount].y = Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].y + 25;

				//	saveRightTri[saveCount].x = SliceTriangle[1].x;

				saveCount++;
			}
			SliceTriangle[0].end = TRUE;
		}
		
		if (SliceTriangle[1].count == 100 && SliceTriangle[1].end == FALSE) {

			if (SliceTriangle[1].SaveOn == TRUE) {
				saveLeftTri[saveCount2].x = Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].x - 25;
				//	saveRightTri[saveCount].y = SliceTriangle[1].y;
				saveLeftTri[saveCount2].y = Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].y - 25;
				saveCount2++;
			}
			SliceTriangle[1].end = TRUE;
		}

		if (SliceTriangle[0].count >= 100 && SliceTriangle[1].count >= 100) {
			
			SliceTriangle[0].rad = 0;
			SliceTriangle[1].rad = 0;
			rect.slice = 0;
			rect.y = -25;
			SliceTriangle[0].count = 0;
			SliceTriangle[1].count = 0;
			slice = FALSE;
			clip = FALSE;
			starCheck = FALSE;
			SliceTriangle[0].SaveOn = TRUE;
			SliceTriangle[1].SaveOn = TRUE;

			SliceTriangle[0].end = FALSE;
			SliceTriangle[1].end = FALSE;
			BlineNumber = 2;

			SliceTriangle[0].click = FALSE;
			SliceTriangle[1].click = FALSE;
		}
	}

	for (int i = 0; i < starcount; ++i) {
		mystar[i].R = (mystar[i].R + 1) % 255;
		mystar[i].G = (mystar[i].G + 1) % 255;
		mystar[i].B = (mystar[i].B + 1) % 255;

		mystar[i].rad = (mystar[i].rad + 1) % 360;
		if (mystar[i].updown == FALSE) {
			mystar[i].size += 0.1;
			if (mystar[i].size >= 1.5)
				mystar[i].updown = TRUE;
		}
		else if (mystar[i].updown == TRUE) {
			mystar[i].size -= 0.1;
			if (mystar[i].size <= 0.7)
				mystar[i].updown = FALSE;
		}
		
	}

	glutTimerFunc(10, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		camera.rotateEye(1, 0, 0);
		break;
	case 'X':
		camera.rotateEye(-1, 0, 0);
		break;

	case 'y':
		camera.rotateEye(0, 1, 0);
		break;
	case 'Y':
		camera.rotateEye(0, -1, 0);
		break;

	case 'z':
		camera.rotateEye(0, 0, 1);
		break;
	case 'Z':
		camera.rotateEye(0, 0, -1);
		break;

		//move
	case 'w':
		camera.moveEye(0, 1, 0);
		break;
	case 'a':
		camera.moveEye(-1, 0, 0);
		break;

	case 's':
		camera.moveEye(0, -1, 0);
		break;

	case 'd':
		camera.moveEye(1, 0, 0);
		break;
	case '+':
		camera.moveEye(0, 0, 1);
		break;
	case '-':
		camera.moveEye(0, 0, -1);
		break;
	case 'i':
		camera.Init();
		break;
	}
}

static float xRate;
static float yRate;
static float totalRate;

static int clipNumber = 0;

void Mouse(int button, int state, int x, int y)
{
	if (slice == FALSE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			ML.startX = x;
			ML.startY = 600 - 1 - y;
			ML.endX = x;
			ML.endY = 600 - 1 - y;
			left = true;
			SliceTriangle[0].click = FALSE;
			SliceTriangle[0].click = FALSE;
			starCheck = FALSE;
		}


		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			ML.endX = x;
			ML.endY = 600 - 1 - y;

			xRate = ML.endX - ML.startX;
			yRate = ML.endY - ML.startY;

			totalRate = yRate / xRate;
			point a1;
			point a2;
			point b1;
			point b2;

			a1.x = ML.startX;
			a1.y = ML.startY;

			a2.x = ML.endX;
			a2.y = ML.endY;

			if (ML.startX < rect.x - 25) {
				b1.x = rect.x - 25;
				b1.y = rect.y + 25;

				b2.x = rect.x - 25;
				b2.y = rect.y - 25;
			}

			else {
				b1.x = rect.x + 25;
				b1.y = rect.y + 25;

				b2.x = rect.x + 25;
				b2.y = rect.y - 25;
			}

			if (check_intersect(&a1, &a2, &b1, &b2) == 1) {
				if (totalRate < 0)
					rect.slice = 1;
				else
					rect.slice = 2;

			}

			b1.x = rect.x - 25;
			b1.y = rect.y + 25;
			b2.x = rect.x + 25;
			b2.y = rect.y + 25;

			if (check_intersect(&a1, &a2, &b1, &b2) == 1) {
				if (totalRate < 0)
					rect.slice = 1;
				else
					rect.slice = 2;
			}

			b1.x = rect.x - 25;
			b1.y = rect.y - 25;
			b2.x = rect.x + 25;
			b2.y = rect.y - 25;

			if (check_intersect(&a1, &a2, &b1, &b2) == 1) {
				if (totalRate < 0)
					rect.slice = 1;
				else if (totalRate >= 0)
					rect.slice = 2;
			}

			if (rect.slice == 1) {
				slice = TRUE;
				bool yDcheck = FALSE;
				bool Destinationcheck = false;
				int yD = 0;
				SliceTriangle[0].x = rect.x - 25;
				SliceTriangle[0].y = rect.y - 25;
				SliceTriangle[0].destinationX = rand() % 7;
				while (Destinationcheck == false) {
					if (Leftsave[SliceTriangle[0].destinationX][yD].leftPiece == FALSE) {
						SliceTriangle[0].destinationY = yD;
						Destinationcheck = true;
						yD = 0;
					}
					else {
						yD++;
						if (yD > 2) {
							yDcheck = TRUE;
							Destinationcheck = true;
							yD = 0;
						}
					}
				}

				if (yDcheck == FALSE) {
					Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].leftPiece = true;
					SliceTriangle[0].xMove = ((Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].x - 25) - SliceTriangle[0].x) / 100;
					SliceTriangle[0].yMove = ((Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].y - 25) - SliceTriangle[0].y) / 100;
					SliceTriangle[0].radMove = 3.6;
				}

				else if (yDcheck == TRUE) {
					int tempX = rand() % 800;
					SliceTriangle[0].xMove = (tempX - SliceTriangle[0].x) / 100;
					SliceTriangle[0].yMove = (-100 - SliceTriangle[0].y) / 100;
					SliceTriangle[0].radMove = 3.6;
					SliceTriangle[0].SaveOn = FALSE;
				}

				yDcheck = FALSE;
				Destinationcheck = false;
				SliceTriangle[1].x = rect.x + 25;
				SliceTriangle[1].y = rect.y + 25;
				SliceTriangle[1].destinationX = rand() % 7;
				while (Destinationcheck == false) {
					if (Rightsave[SliceTriangle[1].destinationX][yD].RightPiece == FALSE) {
						SliceTriangle[1].destinationY = yD;
						Destinationcheck = true;
						yD = 0;
					}
					else {
						yD++;
						if (yD > 2) {
							yDcheck = TRUE;
							Destinationcheck = true;
							yD = 0;
						}
					}
				}

				if (yDcheck == FALSE) {
					Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].RightPiece = true;
					SliceTriangle[1].radMove = 3.6;
					SliceTriangle[1].xMove = ((Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].x + 25) - SliceTriangle[1].x) / 100;
					SliceTriangle[1].yMove = ((Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].y + 25) - SliceTriangle[1].y) / 100;
				}
				else if (yDcheck == TRUE) {
					int tempX = rand() % 800;
					SliceTriangle[1].xMove = (tempX - SliceTriangle[0].x) / 100;
					SliceTriangle[1].yMove = (-100 - SliceTriangle[0].y) / 100;
					SliceTriangle[1].radMove = 3.6;
					SliceTriangle[1].SaveOn = FALSE;
				}
			}

			else if (rect.slice == 2) {
				slice = TRUE;
				bool Destinationcheck = false;
				bool yDcheck = FALSE;
				int yD = 0;
				SliceTriangle[0].x = rect.x - 25;
				SliceTriangle[0].y = rect.y + 25;
				SliceTriangle[0].destinationX = rand() % 7;
				while (Destinationcheck == false) {
					if (Leftsave[SliceTriangle[0].destinationX][yD].RightPiece == FALSE) {
						SliceTriangle[0].destinationY = yD;
						Destinationcheck = true;
						yD = 0;
					}
					else {
						yD++;
						if (yD > 2) {
							yDcheck = TRUE;
							Destinationcheck = true;
							yD = 0;
						}
					}
				}

				if (yDcheck == FALSE) {
					Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].RightPiece = true;
					SliceTriangle[0].radMove = 4.5;
					SliceTriangle[0].xMove = ((Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].x + 25) - SliceTriangle[0].x) / 100;
					SliceTriangle[0].yMove = ((Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].y + 25) - SliceTriangle[0].y) / 100;
				}

				else if (yDcheck == TRUE) {
					int tempX = rand() % 800;
					SliceTriangle[0].xMove = (tempX - SliceTriangle[0].x) / 100;
					SliceTriangle[0].yMove = (-100 - SliceTriangle[0].y) / 100;
					SliceTriangle[0].radMove = 4.5;
					SliceTriangle[0].SaveOn = FALSE;
				}

				yDcheck = FALSE;
				Destinationcheck = false;
				SliceTriangle[1].x = rect.x + 25;
				SliceTriangle[1].y = rect.y - 25;
				SliceTriangle[1].destinationX = rand() % 7;
				while (Destinationcheck == false) {
					if (Rightsave[SliceTriangle[1].destinationX][yD].leftPiece == FALSE) {
						SliceTriangle[1].destinationY = yD;
						Destinationcheck = true;
						yD = 0;
					}
					else {
						yD++;
						if (yD > 2) {
							yDcheck = TRUE;
							Destinationcheck = true;
							yD = 0;
						}
					}
				}

				if (yDcheck == FALSE) {
					Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].leftPiece = true;
					SliceTriangle[1].radMove = 4.5;
					SliceTriangle[1].xMove = ((Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].x - 25) - SliceTriangle[1].x) / 100;
					SliceTriangle[1].yMove = ((Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].y - 25) - SliceTriangle[1].y) / 100;
				}

				else if (yDcheck == TRUE) {
					int tempX = rand() % 800;
					SliceTriangle[1].xMove = (tempX - SliceTriangle[1].x) / 100;
					SliceTriangle[1].yMove = (-100 - SliceTriangle[1].y) / 100;
					SliceTriangle[1].radMove = 4.5;
					SliceTriangle[1].SaveOn = FALSE;
				}

			}

			left = false;
		}
	}

	else if (slice == TRUE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (rect.slice == 1) {
				if (SliceTriangle[0].x - 25 < x && SliceTriangle[0].x + 25 > x && SliceTriangle[0].y - 25 < (600 - 1 - y) && SliceTriangle[0].y + 25 > (600 - 1 - y)) {
					SliceTriangle[0].click = TRUE;
					clip = TRUE;
					SliceTriangle[0].SaveOn = FALSE;
					clipNumber = 0;
				}
				else if ((SliceTriangle[1].x - 25 < x && SliceTriangle[1].x + 25 > x && SliceTriangle[1].y - 25 < (600 - 1 - y) && SliceTriangle[1].y + 25 > (600 - 1 - y))) {
					SliceTriangle[1].click = TRUE;
					clip = TRUE;
					SliceTriangle[1].SaveOn = FALSE;
					clipNumber = 1;
				}
			}
			
			else if (rect.slice == 2) {
				if (SliceTriangle[0].x - 25 < x && SliceTriangle[0].x + 25 > x && SliceTriangle[0].y - 25 < (600 - 1 - y) && SliceTriangle[0].y + 25 > (600 - 1 - y)) {
					SliceTriangle[0].click = TRUE;
					clip = TRUE;
					SliceTriangle[0].SaveOn = FALSE;
					clipNumber = 0;
				}
				else if (SliceTriangle[1].x - 25 < x && SliceTriangle[1].x + 25 > x && SliceTriangle[1].y - 25 < (600 - 1 - y) && SliceTriangle[1].y + 25 > (600 - 1 - y)) {
					SliceTriangle[1].click = TRUE;
					clip = TRUE;
					SliceTriangle[1].SaveOn = FALSE;
					clipNumber = 1;
				}
			}
		}

		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			if (SliceTriangle[0].click == TRUE) {

				if (rect.slice == 1) {
					for (int i = 0; i < 8; ++i) {
						if (Tri[i].x - 25 < x && Tri[i].x + 25 > x && Tri[i].y - 25 < 600 - 1 - y && Tri[i].y + 25 > 600 - 1 - y) {
							starCheck = TRUE;
							mystar[starcount].x = Tri[i].x;
							mystar[starcount].y = Tri[i].y;
							mystar[starcount].destinationX = rand() % 600 + 100;
							mystar[starcount].destinationY = rand() % 300 + 200;
							mystar[starcount].moveX = (mystar[starcount].destinationX - mystar[starcount].x) / 100;
							mystar[starcount].moveY = (mystar[starcount].destinationY - mystar[starcount].y) / 100;
							mystar[starcount].count = 0;
							mystar[starcount].R = rand() % 255;
							mystar[starcount].G = rand() % 255;
							mystar[starcount].B = rand() % 255;
							BlineNumber = clipNumber;
							Tri[i].visible = FALSE;
							starcount++;
						}
					}
					if (starCheck == FALSE) {
						int tempX = rand() % 800;
						SliceTriangle[0].xMove = (tempX - SliceTriangle[0].x) / 100;
						SliceTriangle[0].yMove = (-100 - SliceTriangle[0].y) / 100;
						SliceTriangle[0].radMove = 3.6;
						SliceTriangle[0].count = 0;
						SliceTriangle[0].click = FALSE;
						clip = FALSE;
					}
				}
				else if (rect.slice == 2) {
					for (int i = 0; i < 8; ++i) {
						if (Tri[i].x - 25 < x && Tri[i].x + 25 > x && Tri[i].y - 25 < 600 - 1 - y && Tri[i].y + 25 > 600 - 1 - y) {
							starCheck = TRUE;
							starCheck = TRUE;
							mystar[starcount].x = Tri[i].x;
							mystar[starcount].y = Tri[i].y;
							mystar[starcount].destinationX = rand() % 600 + 100;
							mystar[starcount].destinationY = rand() % 300 + 200;
							mystar[starcount].moveX = (mystar[starcount].destinationX - mystar[starcount].x) / 100;
							mystar[starcount].moveY = (mystar[starcount].destinationY - mystar[starcount].y) / 100;
							mystar[starcount].count = 0;
							mystar[starcount].R = rand() % 255;
							mystar[starcount].G = rand() % 255;
							mystar[starcount].B = rand() % 255;
							Tri[i].visible = FALSE;
							BlineNumber = clipNumber;
							starcount++;
						}
					}
					if (starCheck == FALSE) {
						int tempX = rand() % 800;
						SliceTriangle[0].xMove = (tempX - SliceTriangle[0].x) / 100;
						SliceTriangle[0].yMove = (-100 - SliceTriangle[0].y) / 100;
						SliceTriangle[0].radMove = 3.6;
						SliceTriangle[0].count = 0;
						SliceTriangle[0].click = FALSE;
						clip = FALSE;
					}
				}
			}

			else if (SliceTriangle[1].click == TRUE) {
				if (rect.slice == 1) {
					for (int i = 0; i < 8; ++i) {
						if (Tri[i].x - 25 < x && Tri[i].x + 25 > x && Tri[i].y - 25 < 600 - 1 - y && Tri[i].y + 25 > 600 - 1 - y) {
							starCheck = TRUE;
							mystar[starcount].x = Tri[i].x;
							mystar[starcount].y = Tri[i].y;
							mystar[starcount].destinationX = rand() % 600 + 100;
							mystar[starcount].destinationY = rand() % 300 + 200;
							mystar[starcount].moveX = (mystar[starcount].destinationX - mystar[starcount].x) / 100;
							mystar[starcount].moveY = (mystar[starcount].destinationY - mystar[starcount].y) / 100;
							mystar[starcount].count = 0;
							mystar[starcount].R = rand() % 255;
							mystar[starcount].G = rand() % 255;
							mystar[starcount].B = rand() % 255;
							Tri[i].visible = FALSE;
							BlineNumber = clipNumber;
							if (clipNumber == 0)
								Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].leftPiece = false;
							else if (clipNumber == 1)
								Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].RightPiece = false;
							starcount++;
						}
					}
					if (starCheck == FALSE) {
						int tempX = rand() % 800;
						SliceTriangle[1].xMove = (tempX - SliceTriangle[1].x) / 100;
						SliceTriangle[1].yMove = (-100 - SliceTriangle[1].y) / 100;
						SliceTriangle[1].radMove = 3.6;
						SliceTriangle[1].count = 0;
						if (clipNumber == 0)
							Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].leftPiece = false;
						else if (clipNumber == 1)
							Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].RightPiece = false;
						SliceTriangle[1].click = FALSE;
						clip = FALSE;
					}
				}
				else if (rect.slice == 2) {
					for (int i = 0; i < 8; ++i) {
						if (Tri[i].x - 25 < x && Tri[i].x + 25 > x && Tri[i].y - 25 < 600 - 1 - y && Tri[i].y + 25 > 600 - 1 - y) {
							starCheck = TRUE;
							mystar[starcount].x = Tri[i].x;
							mystar[starcount].y = Tri[i].y;
							mystar[starcount].destinationX = rand() % 600 + 100;
							mystar[starcount].destinationY = rand() % 300 + 200;
							mystar[starcount].moveX = (mystar[starcount].destinationX - mystar[starcount].x) / 100;
							mystar[starcount].moveY = (mystar[starcount].destinationY - mystar[starcount].y) / 100;
							mystar[starcount].count = 0;
							mystar[starcount].R = rand() % 255;
							mystar[starcount].G = rand() % 255;
							mystar[starcount].B = rand() % 255;
							Tri[i].visible = FALSE;
							BlineNumber = clipNumber;
							if (clipNumber == 0)
								Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].RightPiece = false;
							else if (clipNumber == 1)
								Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].leftPiece = false;
							starcount++;
						}
					}
					if (starCheck == FALSE) {
						int tempX = rand() % 800;
						SliceTriangle[1].xMove = (tempX - SliceTriangle[1].x) / 100;
						SliceTriangle[1].yMove = (-100 - SliceTriangle[1].y) / 100;
						SliceTriangle[1].radMove = 3.6;
						SliceTriangle[1].count = 0;
						if (clipNumber == 0)
							Leftsave[SliceTriangle[0].destinationX][SliceTriangle[0].destinationY].RightPiece = false;
						else if (clipNumber == 1)
							Rightsave[SliceTriangle[1].destinationX][SliceTriangle[1].destinationY].leftPiece = false;
						SliceTriangle[1].click = FALSE;
						clip = FALSE;
					}
				}
			}
			clip = FALSE;
		}
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	if (left == true) {
		ML.endX = x;
		ML.endY = 600 - 1 - y;
	}

	if (clip == TRUE) {
		SliceTriangle[clipNumber].x = x;
		SliceTriangle[clipNumber].y = 600 - 1 - y;
	}
}


int check_intersect(struct point * pA, struct point * pB, struct point * pC, struct point * pD)
{
	if (MAX(Ax, Bx) < MIN(Cx, Dx)) return 0;
	if (MIN(Ax, Bx) > MAX(Cx, Dx)) return 0;
	if (MAX(Ay, By) < MIN(Cy, Dy)) return 0;
	if (MIN(Ay, By) > MAX(Cy, Dy)) return 0;

	/* line AB & segment CD */
#define f_AB(x,y) ((y - Ay) * (Bx - Ax) - (x - Ax) * (By - Ay))
	if ((f_AB(Cx, Cy) * f_AB(Dx, Dy)) > 0) return 0;

	/* line CD & segment AB */
#define f_CD(x,y) ((y - Cy) * (Dx - Cx) - (x - Cx) * (Dy - Cy))
	if ((f_CD(Ax, Ay) * f_CD(Bx, By)) > 0) return 0;

	return 1;
}

void processMousePassiveMotion(int x, int y)
{
	
	
}