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
void SpecialKeys(int key, int x, int y);
static int mode = 2;

static int select = -100;


GLfloat ctrlpoints[10][3][3] = { 0, 0, 0 };

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

static int count = 0;
static Cam camera;
void Mouse(int button, int state, int x, int y);

void SetupRC()
{
	camera.rotateEye(0, 90, 0);

	for(int i = 0; i < 10; ++i)
		for (int j = 0; j < 3; ++j) {
			ctrlpoints[i][j][0] = -1000;
			ctrlpoints[i][j][1] = -1000;
			ctrlpoints[i][j][2] = -1000;
		}
}

void main(int agrc, char *argv[]) { // 윈도우 초기화 및 생성 
	glutInit(&agrc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// 더블 버퍼링과 3차원으로 그린다. 
	glutInitWindowSize(800, 600);
	glutCreateWindow("Points Drawing");
	// 상태 변수 초기화 함수 
	SetupRC();
	// 필요한 콜백 함수 설정
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene); // 출력 콜백 함수 
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	glutTimerFunc(100, TimerFunction, 1); // 타이머 콜백 함수
	glutSpecialFunc(SpecialKeys);
	glutMainLoop(); // 이벤트 루프 실행하기
}

static BOOL check = FALSE;
GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	camera.drawCamera();

	if (check == TRUE) {
		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[0][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		// 그리드를 이용한 곡면 드로잉 
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		// 선을 이용하여 그리드 연결
		glEvalMesh2(GL_LINE, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_VERTEX_3);

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[2][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		// 그리드를 이용한 곡면 드로잉 
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		// 선을 이용하여 그리드 연결
		glEvalMesh2(GL_LINE, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_VERTEX_3);

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[4][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		// 그리드를 이용한 곡면 드로잉 
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		// 선을 이용하여 그리드 연결
		glEvalMesh2(GL_LINE, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_VERTEX_3);

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[6][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		// 그리드를 이용한 곡면 드로잉 
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		// 선을 이용하여 그리드 연결
		glEvalMesh2(GL_LINE, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_VERTEX_3);

		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[7][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		// 그리드를 이용한 곡면 드로잉 
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		// 선을 이용하여 그리드 연결
		glEvalMesh2(GL_LINE, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_VERTEX_3);
	}
	
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 3; j++)
			glVertex3fv(ctrlpoints[i][j]);

	glEnd();

	glPopMatrix();
	glutSwapBuffers();
	//	glFlush(); // 화면에 출력하기 
}



static float zZoom = 45;
GLvoid Reshape(int w, int h)
{
	GLfloat nRange = 800.0f;

	glViewport(-1, -1, w, h);  // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (mode == 1) {
		//-- 투영은 직각 투영 또는 원근 투영 중 한 개를 설정한다. // 1. 클리핑 공간 설정: 원근 투영인 경우
		gluPerspective(zZoom, 1.0, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -500.0);     // 투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
	}
	else if (mode == 2)
		glOrtho(-400.0, 400.0, -300.0, 300.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void TimerFunction(int value)
{
	glutPostRedisplay();

	glutTimerFunc(1, TimerFunction, 1);
}
static BOOL plusDir = FALSE;
static int opic = 0;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	case 'c':
		if (plusDir == FALSE)
			plusDir = TRUE;
		else
			plusDir = FALSE;
		break;

	case 'n':
		opic = (opic + 1) % 3;
		break;

	case 'k':
		if (mode == 1) {
			mode = 2;
			Reshape(800, 600);
		}
		else if (mode == 2) {
			mode = 1;
			Reshape(800, 600);
		}
		break;

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

	case '9':
		zZoom += 1;
		Reshape(800, 600);
		break;

	case '0':
		zZoom -= 1;
		Reshape(800, 600);
		break;

	case 'q':
		PostQuitMessage(0);
		break;
	}
}



static int swi = 0;
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (check == FALSE) {
			if (count < 12) {
				if (swi == 0) {
					ctrlpoints[count][0][0] = x - 400;
					ctrlpoints[count][0][1] = 0;
					ctrlpoints[count][0][2] = -(300 - 1 - y);

				}
				else {
					ctrlpoints[count][2][0] = x - 400;
					ctrlpoints[count][2][1] = 0;
					ctrlpoints[count][2][2] = -(300 - 1 - y);
				}
				swi = (swi + 1) % 2;
				if (swi == 0)
					count += 3;
			}

			if (count == 12 && check == FALSE) {
				for (int i = 0; i <= 6; i += 3) {
					ctrlpoints[i + 1][0][0] = ctrlpoints[i][0][0] + (float)(ctrlpoints[i + 3][0][0] - ctrlpoints[i][0][0]) / 3;
					ctrlpoints[i + 1][2][0] = ctrlpoints[i][2][0] + (float)(ctrlpoints[i + 3][2][0] - ctrlpoints[i][2][0]) / 3;

					ctrlpoints[i + 1][0][2] = ctrlpoints[i][0][2] + (float)(ctrlpoints[i + 3][0][2] - ctrlpoints[i][0][2]) / 3;
					ctrlpoints[i + 1][2][2] = ctrlpoints[i][2][2] + (float)(ctrlpoints[i + 3][2][2] - ctrlpoints[i][2][2]) / 3;

					ctrlpoints[i + 2][0][0] = ctrlpoints[i][0][0] + ((float)(ctrlpoints[i + 3][0][0] - ctrlpoints[i][0][0]) / 3) * 2;
					ctrlpoints[i + 2][2][0] = ctrlpoints[i][2][0] + ((float)(ctrlpoints[i + 3][2][0] - ctrlpoints[i][2][0]) / 3) * 2;

					ctrlpoints[i + 2][0][2] = ctrlpoints[i][0][2] + ((float)(ctrlpoints[i + 3][0][2] - ctrlpoints[i][0][2]) / 3) * 2;
					ctrlpoints[i + 2][2][2] = ctrlpoints[i][2][2] + ((float)(ctrlpoints[i + 3][2][2] - ctrlpoints[i][2][2]) / 3) * 2;

					ctrlpoints[i + 1][0][1] = 0;
					ctrlpoints[i + 2][2][1] = 0;
				}

				for (int i = 0; i < 10; ++i) {
					ctrlpoints[i][1][0] = ctrlpoints[i][0][0] + ((float)(ctrlpoints[i][2][0] - ctrlpoints[i][0][0]) / 2);
					ctrlpoints[i][1][2] = ctrlpoints[i][0][2] + ((float)(ctrlpoints[i][2][2] - ctrlpoints[i][0][2]) / 2);
					ctrlpoints[i][1][1] = 0;
				}

				for (int i = 0; i < 10; ++i)
					for (int j = 0; j < 3; ++j)
						ctrlpoints[i][j][1] = 0;
				check = TRUE;
			}
		}

	}
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		select = 1;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F2) {
		select = 2;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F3) {
		select = 3;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F4) {
		select = 4;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F5) {
		select = 5;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F6) {
		select = 6;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F7) {
		select = 7;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F8) {
		select = 8;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F9) {
		select = 9;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}

	if (key == GLUT_KEY_F10) {
		select = 0;
		if (plusDir == FALSE) {
			ctrlpoints[select][opic][1] += 1;
		}
		else
			ctrlpoints[select][opic][1] -= 1;
	}
	glutPostRedisplay();
}