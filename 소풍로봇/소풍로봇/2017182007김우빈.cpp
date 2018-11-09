#include <gl/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <gl\freeglut.h>
#include <math.h>

#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DXToRadian(degree)((degree) * (D3DX_PI / 180.f))
#define D3DXToDegree(radian)((radian) * (180.f / D3DX_PI))



// 원에 십자를 그었을때 ┾ 두꺼운 선이 각도 0도이고 시계방향으로 1도, 2도... 한바퀴 돌아 두꺼운 선직전까지 360도이다.

// 각도 = 기준 점, 확인 할 점

static float GetDirectionAngle(float x1, float y1, float x2, float y2)
{
	x2 -= x1;
	y2 -= y1;
	//float Angle = D3DXToDegree( atan2(y2, x2) );

	float Angle = atan2(y2, x2) * 57.3f;

	//if (0 > Angle)
	//	Angle += 360;
	return Angle;
}

GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 1;

static int select = -100;
static int right = 0;
static BOOL change = FALSE;
static int changeCount = 0;
static BOOL chOn = FALSE;


GLfloat ctrlpoints[80][3] = { 0, 0, 0 };
GLfloat ctrlpoints2[80][3] = { 0,0,0 };

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


static Cam camera;
static Cam topView;
static int cameraNum;
static int count = 0;
static int drawTrue = 0;


void Mouse(int button, int state, int x, int y);

void SetupRC()
{
	camera.rotateEye(0, 40, 0);
	topView.rotateEye(0, 90, 0);
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
	


GLvoid drawPlan(GLvoid)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	{
		glPushMatrix();
		//	glRotatef(30, 1, 0, 0);
		glScalef(1, 0.01, 1);
		glutSolidCube(400);
		glPopMatrix();
	}
}

GLvoid DrawTree(float x, float z)
{

}

GLvoid drawRail(GLvoid)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	BOOL check = FALSE;
	int i = 0;
	int temp = count - 1;
	check = FALSE;
	i = 0;
	
	while (check != TRUE) {
		if (count > 3) {
			glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints[i][0]);

			glEnable(GL_MAP1_VERTEX_3);

			glMapGrid1f(10, 0.0, 1.0);
			glEvalMesh1(GL_LINE, 0, 10);
		}

		glDisable(GL_MAP1_VERTEX_3);
		i += 2;
		if (i >= count && i + 1 >= count || i + 2 >= count || i + 3 >= count)
			check = TRUE;
	}


	//check = FALSE;
	//i = 0;
	//while (check != TRUE) {
	//	if (count > 3) {
	//		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints2[i][0]);

	//		glEnable(GL_MAP1_VERTEX_3);

	//		glMapGrid1f(10, 0.0, 1.0);
	//		glEvalMesh1(GL_LINE, 0, 10);
	//	}

	//	glDisable(GL_MAP1_VERTEX_3);
	//	i += 3;
	//	if (i >= count && i + 1 >= count || i + 2 >= count || i + 3 >= count)
	//		check = TRUE;
	//}

	glPointSize(5.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for(int i = 0; i < count; ++i)
		for(int j = 0; j < 2; ++j)
			glVertex3fv(ctrlpoints[i]);
	glEnd();
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();


	if (cameraNum == 0)
		camera.drawCamera();
	else
		topView.drawCamera();


	drawRail();

	drawPlan();



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


void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
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

	case '1':
		mode = 1;
		Reshape(800, 600);
		cameraNum = 0;
		break;

	case '2':
		mode = 2;
		Reshape(800, 600);
		cameraNum = 1;
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

static float rad;

void Mouse(int button, int state, int x, int y)
{

	if (cameraNum == 1) {




		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			ctrlpoints[count][0] = x - 400;
			ctrlpoints[count][1] = 100;
			ctrlpoints[count][2] = -(300 - 1 - y);
			if (count % 2 == 0 && count >= 2) {
				rad = GetDirectionAngle(ctrlpoints[count - 2][0], ctrlpoints[count - 2][2], ctrlpoints[count][0], ctrlpoints[count][2]);
				ctrlpoints[count - 1][0] = ctrlpoints[count][0] + (sin((3.141592 / 180.f) * (180 + (rad))) * 30);
				ctrlpoints[count - 1][1] = 100;
				ctrlpoints[count - 1][2] = ctrlpoints[count][2] + (sin((3.141592 / 180.f) * (180 + (rad))) * 100);
			}

			count += 2;
		}




	}
	glutPostRedisplay();

}






void SpecialKeys(int key, int x, int y) {

	glutPostRedisplay();
}