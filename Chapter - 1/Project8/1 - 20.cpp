#include <GL/freeglut.h>     // �Ǵ� <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//��, ����ü, ����, ������
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

//bool ����.
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
int Point[pointArrSize][2] = { 0 }; // Shape ��ġ ����

void SetupRC();

/* �Է� �Լ� ����*/
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

/*Draw �Լ� ����*/
GLvoid DrawGround();
GLvoid DrawCoordinate();
GLvoid Draw_Low();
GLvoid Draw_Mid();
GLvoid Draw_Top();

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);

void main(int argc, char** argv)    // ������ ����ϰ� ����Լ� ����
{   // ������ �ʱ�ȭ �� ����
	glutInit(&argc, argv);    // glut �ʱ�ȭ        
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  // ���÷��� ��� ����        
	glutInitWindowPosition(100, 100);   // �������� ��ġ ����
	glutInitWindowSize(XSize, YSize);    // �������� ũ�� ����
	glutCreateWindow("Defualt");   // ������ ���� (������ �̸�)  
	srand(time(NULL));//���� �Լ� �ʱ�ȭ.

	SetupRC();// ���� ���� �ʱ�ȭ �Լ�


			  //�ݹ� �Լ� ����.
	glutDisplayFunc(drawScene);    // ��� �Լ��� ����
	glutMouseFunc(Mouse);//���콺 �Է� �Լ� ����
	glutKeyboardFunc(Keyboard);//Ű���� �Է� �Լ� ����
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(100, TimerFunction, 1);//Ÿ�̸� �Լ� ����
	glutReshapeFunc(reshape);   // �ٽ� �׸��� �Լ� ����        


	glutMainLoop();  // �̺�Ʈ ó�� ���� 

}

/* �ݹ� �Լ� */
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
	glutPostRedisplay();// ȭ�� ������� ���� ���÷��� �ݹ� �Լ� ȣ��.
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

	glutPostRedisplay();// ȭ�� ������� ���� ���÷��� �ݹ� �Լ� ȣ��.
	glutTimerFunc(100, TimerFunction, 1);
}

/* �׸��� �Լ� */
GLvoid drawScene()     // ��� �Լ� 
{
	//��ȯ + �׸��� ����.
	/*
	1. glPushMatrix �Լ� ȣ��� '������ ��ǥ �ý����� ����.'
	2. �ʿ��� ��� glLoadIdentity();�� '��� �ʱ�ȭ.'
	3. '��ȯ �Լ� ȣ��' (�̵�, ȸ��, ����)
	4. ��ȯ�� ���� �Ŀ��� ������ ��ǥ �ý����� �ٽ� �����ϱ� ���ؼ� glPopMatrix(); �Լ� ȣ��.
	5. �ʿ��� �׸��� �۾� ����.
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

	glutSwapBuffers(); //���� ��ȯ (��� ���)
}
GLvoid reshape(int w, int h)    // �ٽ� �׸��� �Լ�
{
	//����Ʈ ��ȯ ����.
	glViewport(0, 0, w, h);

	//���� ��� ���� �缳��.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//���� ���� : ���� ����, ���� ����.

	//���� ����
	gluPerspective(60.0f, w / h, 100.0, 1000.0);
	//glTranslatef(0.0, 0.0, -100.0);
	//gluLookAt(0.0, 0.5, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, -1.0);
	//���� ����
	//glOrtho(-XSize, XSize, -YSize, XSize, -ZSize, ZSize);



	//�� �� ��� ���� �缳��.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//���� ��ȯ : ī�޶��� ��ġ ����

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

GLvoid DrawCoordinate()//F1 Ű �Է½� ��ǥ�� ����.
{
	if (Coordi)
	{
		glPushMatrix();
		glTranslated(0, 20, 0);
		glBegin(GL_LINES);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, CSize);
		//Z��
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(CSize, 0, 0);
		//X�� (RED)
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, CSize, 0);
		//Y��
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
	//�ʿ��� ������, ��ǥ�� ���� �ʱ�ȭ.
	//��� ���� �ʱ�ȭ.
}
