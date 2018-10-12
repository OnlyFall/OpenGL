#include <GL/freeglut.h>     // 또는 <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//구, 육면체, 원뿔, 주전자
#define Min(x, y) (((x) > (y)) ? (x) : (y))


#define XSize 600
#define YSize 600
#define ZSize 600
#define CSize 20
#define pi 3.14
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15

class Cam {
public:
	void Rotatex() { if (Angle_X < 90) Angle_X++; }
	void RotateX() { if (Angle_X > -90) Angle_X--; }
	void Rotatey() { if (Angle_Y < 90) Angle_Y++; }
	void RotateY() { if (Angle_Y > -90) Angle_Y--; }
	void Rotatez() { if (Angle_Z < 90) Angle_Z++; }
	void RotateZ() { if (Angle_Z > -90) Angle_Z--; }
	void Up() { eyeY -= 1;	centerY -= 1; }
	void Down() { eyeY += 1;	centerY += 1; }
	void Left() { eyeX += 1;	centerX += 1; }
	void Right() { eyeX -= 1;	centerX -= 1; }
	void Expand() { eyeZ -= 1; }
	void Reduce() { eyeZ += 1; }
	void Init() {
		eyeX = 0.0;
		eyeY = 20.0;
		eyeZ = 100.0;
		centerX = 0.f;
		centerY = 0.f;
		centerZ = -1.f;
		upX = 0.f;
		upY = 1.f;
		upZ = -1.f;
		Angle_X = 0.0;
		Angle_Y = 0.0;
		Angle_Z = 0.0;
	}
public:
	GLfloat eyeX;
	GLfloat eyeY;
	GLfloat eyeZ;
	GLfloat centerX;
	GLfloat centerY;
	GLfloat centerZ;
	GLfloat upX;
	GLfloat upY;
	GLfloat upZ;

	GLdouble Angle_X;
	GLdouble Angle_Y;
	GLdouble Angle_Z;
};
static Cam cam;

//bool 변수.
bool Coordi;
bool Show;
bool DiX;


double angle;
int direction;
int LoAdder;
int LoA = 0;
int LoX = 0;
int MidXA = 0;
int MidYA = 0;
int TopXA = 0;
int TopZA = 0;
int nPoint = 0;
int Point[pointArrSize][2] = { 0 }; // Shape 위치 지정

void SetupRC();

/* 입력 함수 선언*/
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

/*Draw 함수 선언*/
GLvoid DrawGround();
GLvoid DrawCoordinate();
GLvoid Draw_Low();
GLvoid Draw_Mid();
GLvoid Draw_Top();

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);

void main(int argc, char** argv)    // 윈도우 출력하고 출력함수 설정
{   // 윈도우 초기화 및 생성
	glutInit(&argc, argv);    // glut 초기화        
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  // 디스플레이 모드 설정        
	glutInitWindowPosition(100, 100);   // 윈도우의 위치 지정
	glutInitWindowSize(XSize, YSize);    // 윈도우의 크기 지정
	glutCreateWindow("Defualt");   // 윈도우 생성 (윈도우 이름)  
	srand(time(NULL));//랜덤 함수 초기화.

	SetupRC();// 상태 변수 초기화 함수


			  //콜백 함수 설정.
	glutDisplayFunc(drawScene);    // 출력 함수의 지정
	glutMouseFunc(Mouse);//마우스 입력 함수 지정
	glutKeyboardFunc(Keyboard);//키보드 입력 함수 지정
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(100, TimerFunction, 1);//타이머 함수 지정
	glutReshapeFunc(reshape);   // 다시 그리기 함수 지정        


	glutMainLoop();  // 이벤트 처리 시작 

}

/* 콜백 함수 */
GLvoid Mouse(int button, int state, int x, int y)
{
	//if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	//{
	//	nPoint = nPoint % pointArrSize;
	//	Point[nPoint][0] = x;
	//	Point[nPoint][1] = y;
	//	nPoint++;
	//}
	//if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	//{
	//
	//}
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (key == '[')
	{
		LoAdder -= 2;
	}
	else if (key == ']')
	{
		LoAdder += 2;;
	}
	else if (key == 's')
	{
		if (MidXA < 90)
		{
			MidXA += 2;
		}
	}
	else if (key == 'w')
	{
		if (MidXA > -90)
		{
			MidXA -= 2;
		}
	}
	else if (key == 'd')
	{
		if (MidYA < 90)
		{
			MidYA += 2;
		}
	}
	else if (key == 'a')
	{
		if (MidYA > -90)
		{
			MidYA -= 2;
		}
	}
	else if (key == '8')	cam.Up();
	else if (key == '2')	cam.Down();
	else if (key == '4')	cam.Left();
	else if (key == '6')	cam.Right();
	else if (key == '+')	cam.Expand();
	else if (key == '-')	cam.Reduce();
	else if (key == 'x')	cam.Rotatex();
	else if (key == 'X')	cam.RotateX();
	else if (key == 'y')	cam.Rotatey();
	else if (key == 'Y')	cam.RotateY();
	else if (key == 'z')	cam.Rotatez();
	else if (key == 'Z')	cam.RotateZ();
	else if (key == 'i')	cam.Init();
	glutPostRedisplay();// 화면 재출력을 위해 디스플레이 콜백 함수 호출.
}
GLvoid SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
		Coordi = !Coordi;
	else if (key == GLUT_KEY_UP)
	{
		if (TopXA > -90)
		{
			TopXA -= 2;
		}
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (TopXA < 90)
		{
			TopXA += 2;
		}
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		if (TopZA > -90)
		{
			TopZA -= 2;
		}
	}
	else if (key == GLUT_KEY_LEFT)
	{
		if (TopZA < 90)
		{
			TopZA += 2;
		}
	}
}

GLvoid TimerFunction(int value)
{
	/* Low Move Control*/
	if (!DiX)	LoX += 2;
	else	LoX -= 2;
	if (LoX > 50)	DiX = !DiX;
	else if (LoX < -50)	DiX = !DiX;

	/* Low Angle Control*/
	if (LoA > 360)	LoA = 0;
	else if (LoA < 0)	LoA = 360;
	LoA += LoAdder;

	glutPostRedisplay();// 화면 재출력을 위해 디스플레이 콜백 함수 호출.
	glutTimerFunc(100, TimerFunction, 1);
}

/* 그리기 함수 */
GLvoid drawScene()     // 출력 함수 
{
	//변환 + 그리기 순서.
	/*
	1. glPushMatrix 함수 호출로 '기존의 좌표 시스템을 저장.'
	2. 필요한 경우 glLoadIdentity();로 '행렬 초기화.'
	3. '변환 함수 호출' (이동, 회전, 신축)
	4. 변환이 끝난 후에는 원래의 좌표 시스템을 다시 저장하기 위해서 glPopMatrix(); 함수 호출.
	5. 필요한 그리기 작업 수행.
	*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	gluLookAt(cam.eyeX, cam.eyeY, cam.eyeZ,
		cam.centerX, cam.centerY, cam.centerZ,
		cam.upX, cam.upY, cam.upZ);

	glTranslatef(0.0f, 0.0f, -100.0f);
	glRotated(cam.Angle_X, 1.0, 0.0, 0.0);
	glRotated(cam.Angle_Y, 0.0, 1.0, 0.0);
	glRotated(cam.Angle_Z, 0.0, 0.0, 1.0);
	DrawGround();
	DrawCoordinate();
	glPushMatrix();
	glTranslated(LoX, 0, 0);
	glRotated(LoA, 0.0, 1.0, 0.0);
	Draw_Low();
	Draw_Mid();
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers(); //버퍼 교환 (결과 출력)
}
GLvoid reshape(int w, int h)    // 다시 그리기 함수
{
	//뷰포트 변환 설정.
	glViewport(0, 0, w, h);

	//투영 행렬 스택 재설정.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//투영 선택 : 원근 투영, 직각 투영.

	//원근 투영
	gluPerspective(60.0f, w / h, 100.0, 1000.0);
	//glTranslatef(0.0, 0.0, -100.0);
	//gluLookAt(0.0, 0.5, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, -1.0);
	//직각 투영
	//glOrtho(-XSize, XSize, -YSize, XSize, -ZSize, ZSize);



	//모델 뷰 행렬 스택 재설정.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//관측 변환 : 카메라의 위치 설정

}

GLvoid DrawGround()
{
	if (Coordi)
	{
		glPushMatrix();
		glTranslated(0, -10, 0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex3i(-75, 0, -75);
		glVertex3i(75, 0, -75);
		glVertex3i(75, 0, 75);
		glVertex3i(-75, 0, 75);
		glEnd();
		glPopMatrix();

	}
}

GLvoid DrawCoordinate()//F1 키 입력시 좌표계 꺼짐.
{
	if (Coordi)
	{
		glPushMatrix();
		glTranslated(0, 20, 0);
		glBegin(GL_LINES);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, CSize);
		//Z축
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(CSize, 0, 0);
		//X축 (RED)
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, CSize, 0);
		//Y축
		glEnd();
		glPopMatrix();
	}
}

GLvoid Draw_Low()
{
	glPushMatrix();
	{
		glColor4f(1.0f, 0.7f, 0.1f, 1.0f);
		glScaled(1.5, 0.25, 1.0);
		glutSolidCube(ShapeSize * 2);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(ShapeSize * 2);
	}
	glPopMatrix();
}
GLvoid Draw_Mid()
{
	glPushMatrix();
	glColor4f(1.0f, 0.1f, 0.1f, 1.0f);
	glRotated(MidYA, 0.0, 1.0, 0.0);
	glRotated(MidXA, 1.0, 0.0, 0.0);
	glTranslated(0.0, ShapeSize, 0.0);
	glPushMatrix();
	glScaled(1.0, 4.0, 1.0);

	glutSolidCube(ShapeSize / 2);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutWireCube(ShapeSize / 2);

	glPopMatrix();

	glTranslated(0.0, ShapeSize, 0.0);
	Draw_Top();
	glPopMatrix();
}
GLvoid Draw_Top()
{
	glPushMatrix();
	{
		glColor4f(0.1f, 1.0f, 0.1f, 1.0f);
		glPushMatrix();
		glRotated(TopXA, 1.0, 0.0, 0.0);
		glRotated(TopZA, 0.0, 0.0, 1.0);
		glScaled(1.0, 4.0, 1.0);
		glTranslated(0.0, ShapeSize / 4, 0.0);
		glutSolidCube(ShapeSize / 2);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(ShapeSize / 2);

		glPopMatrix();
	}
	glPopMatrix();
}

void SetupRC()
{
	Coordi = true;
	cam.Init();
	LoAdder = 0;
	LoA = 0;
	LoX = 0;
	MidXA = 0;
	MidYA = 0;
	angle = 0.0;
	Show = false;
	DiX = false;
	//필요한 변수들, 좌표값 등의 초기화.
	//기능 설정 초기화.
}
