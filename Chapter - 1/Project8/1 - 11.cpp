#include <gl\freeglut.h>
#include <math.h>

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
static int shape = 2;

static BOOL rotate = FALSE;
static BOOL animation = FALSE;
static int dirAnimation = 0;

struct Shape {
	double x = 0;
	double y = 0;
	double z = 0;

	double moveX = 0;
	double moveY = 0;	
	double moveZ = 0;

	int count = 0;
};

static GLfloat sizeX = 0.0f;
static GLdouble sizeY = 0.0;


static int count = 0;
static Shape rect[801];
static bool animationOFF = false;
static GLfloat rad = 0;
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
	glutTimerFunc(1, TimerFunction, 1);
	glutReshapeFunc(Reshape); 
	glutMainLoop();
} // 윈도우 출력 함수 

static int triMoveX = 0;
static int triMoveY = 0;

static int rectMoveX = 0;
static int rectMoveY = 0;

static int scaleX = 1;

static int triangle1 = 0;
static int triangle2 = -800;

static int rectleftX = 350, rectleftY = 250;
static int rectrightX = 450, rectrightY = 350;

static int rectleftX2 = -450, rectleftY2 = 250;
static int rectrightX2 = -350 , rectrightY2 = 350;

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

	GLint tri1[2] = { 100 + triMoveX + triangle1, 100 + triMoveY };
	GLint tri2[2] = { 100 + triMoveX + triangle1, 500 + triMoveY };
	GLint tri3[2] = { 400 + triMoveX + triangle1, 300 + triMoveY };
	GLint tri4[2] = { 700 + triMoveX + triangle1, 100 + triMoveY };
	GLint tri5[2] = { 700 + triMoveX + triangle1, 500 + triMoveY };

	GLint tris1[2] = { 100 + triMoveX + triangle2, 100 + triMoveY };
	GLint tris2[2] = { 100 + triMoveX + triangle2, 500 + triMoveY };
	GLint tris3[2] = { 400 + triMoveX + triangle2, 300 + triMoveY };
	GLint tris4[2] = { 700 + triMoveX + triangle2, 100 + triMoveY };
	GLint tris5[2] = { 700 + triMoveX + triangle2, 500 + triMoveY };


	glPushMatrix();
	glTranslatef(400, 0.0f, 0.0f);
	glScalef(scaleX, 1.0f, 1.0f);
	glRotatef(rad, 0, 1, 0);


	glTranslatef(-400, 0.0f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2iv(p1);
	glVertex2iv(p2);
	glVertex2iv(p3);
	glVertex2iv(p4);
	glEnd();
	
	//도형 그리기의 시작
	if (shape == 1) {
		glColor4f((float)255, (float)255, 0.0f, 1.0f);
		for (int i = 0; i <= 800; i += 1) {
			glBegin(GL_POINTS);
			glVertex2f(rect[i].x, rect[i].y);
			glEnd();
		}
	}

	else if (shape == 2) {
		for (double i = 0; i <= 800; i += 1) {
			/*glBegin(GL_POINTS);
			rect.x =  100 * cos(i) + rect.moveX + i;
			rect.y = 100 * sin(i) + 300 + rect.moveY;
			glVertex2f(rect.x, rect.y);
			glEnd();*/
			glColor4f((float)255, (float)255, 0.0f, 1.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i <= 800; i += 1) {
				glVertex3f(rect[i].x, rect[i].y, rect[i].z);
			}
			glEnd();
		}
	}

	else if (shape == 3) {
		glColor4f((float)255, (float)255, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex2iv(tri1);
		glVertex2iv(tri2);
		glVertex2iv(tri3);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2iv(tri3);
		glVertex2iv(tri4);
		glVertex2iv(tri5);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2iv(tris1);
		glVertex2iv(tris2);
		glVertex2iv(tris3);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2iv(tris3);
		glVertex2iv(tris4);
		glVertex2iv(tris5);
		glEnd();
	}

	else if (shape == 4) {
		glColor4f((float)255, (float)255, 0.0f, 1.0f);
		glRectf(rectleftX + rectMoveX, rectleftY + rectMoveY, rectrightX + rectMoveX, rectrightY + rectMoveY);
		glRectf(rectleftX2 + rectMoveX, rectleftY2 + rectMoveY, rectrightX2 + rectMoveX, rectrightY2 + rectMoveY);
	}

	glPopMatrix();
//	glScalef(sizeX, 0.0, 0.0);
	glFlush(); // 화면에 출력하기 

}

GLvoid Reshape(int w, int h)
{
	glViewport(0 , 0, w, h);
	glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		shape = 1;
		for (int i = 0; i < 800; ++i) {
			rect[i].x = i + rect[i].moveX;
			rect[i].y = 100 * sin((rect[i].x / 42.5)) + 300 + rect[i].moveY;
		}
		break;

	case '2':
		shape = 2;
		for (int i = 0; i < 800; ++i) {
			rect[i].x = i + rect[i].moveX;
			rect[i].y = 100 * sin((rect[i].x / 10.f)) + 300 + rect[i].moveY;
			rect[i].z = 100 * cos((rect[i].x / 10.f));
		}
		break;

	case '3':
		shape = 3;
		break;

	case '4':
		shape = 4;
		break;

	case 'x':
		for (int i = 0; i < 800; ++i)
			rect[i].x += 1;

		if (shape == 3) 
			triMoveX += 1;

		if (shape == 4)
			rectMoveX += 1;
		break;

	case 'X':
		for (int i = 0; i < 800; ++i)
			rect[i].x -= 1;
		
		if (shape == 3)
			triMoveX -= 1;

		if (shape == 4)
			rectMoveX -= 1;
		break;

	case 'y':
		for (int i = 0; i < 800; ++i)
			rect[i].y += 1;

		if (shape == 3)
			triMoveY += 1;

		if (shape == 4)
			rectMoveY += 1;
		break;

	case 'Y':
		for (int i = 0; i < 800; ++i)
			rect[i].y -= 1;

		if (shape == 3)
			triMoveY -= 1;

		if (shape == 4)
			rectMoveY -= 1;
		break;

	case 's':
		scaleX += 1.0;
		break;

	case 'S':
		scaleX -= 1.0;
		break;

	case 'r':
		if (rotate == FALSE)
			rotate = TRUE;
		else
			rotate = FALSE;
		break;

	case 'R':

		break;

	case 'a':
		animation = TRUE;
		rectleftX2 = rectleftX - 800;
		rectrightX2 = rectrightX - 800;
		triangle2 = -800;
		dirAnimation = 1;
		break;

	case 'A':
		animation = TRUE;
		rectleftX2 = rectleftX + 800;
		rectrightX2 = rectrightX + 800;
		dirAnimation = 2;
		triangle2 = 800;
		break;

	case 'T':
		animation = FALSE;
		break;

	case 'Q':

		break;
	}


}

static int size = 0;
void TimerFunction(int value)
{
	glutPostRedisplay();
	static int temp;
	if (rotate == TRUE) {
		rad += 10;
	}

	if (animation == TRUE) {
		if (shape == 1 || shape == 2) {
			if (dirAnimation == 1) {
				for (int i = 800; i >= 0; --i)
					rect[i].y = rect[i - 1].y;

				rect[0].y = rect[800].y;
			}

			else if (dirAnimation == 2) {
				for (int i = 0; i < 800; ++i) {
					if (i - 1 < 0)
						temp = rect[i].y;
					else
						rect[i - 1].y = rect[i].y;
				}
				rect[799].y = temp;
			}
		}
		else if (shape == 3) {
			if (dirAnimation == 1) {
				triangle1 += 10;
				triangle2 += 10;

				if (100 + triMoveX + triangle1 >= 800)
					triangle1 = -900;
				if (100 + triMoveX + triangle2 >= 800)
					triangle2 = -900;
			}

			else if(dirAnimation == 2){
				triangle1 -= 10;
				triangle2 -= 10;

				if (700 + triMoveX + triangle1 <= 0)
					triangle1 = 900;
				if (700 + triMoveX + triangle2 <= 0)
					triangle2 = 900;
			}
		}

		else if (shape == 4) {
			if (dirAnimation == 1) {
				rectleftX += 1;
				rectrightX += 1;

				rectleftX2 += 1;
				rectrightX2 += 1;

				if (rectleftX >= 800) {
					rectleftX = -800;
					rectrightX = -700;
				}

				if (rectleftX2 >= 800) {
					rectleftX2 = -800;
					rectrightX2 = -700;
				}
			}

			else if (dirAnimation == 2) {
				rectleftX -= 1;
				rectrightX -= 1;

				rectleftX2 -= 1;
				rectrightX2 -= 1;

				if (rectrightX <= 0) {
					rectleftX = 700;
					rectrightX = 800;
				}

				if (rectrightX2 <= 0) {
					rectleftX2 = 700;
					rectrightX2 = 800;
				}
			}
		}
	}
	glutTimerFunc(100, TimerFunction, 1);
}
