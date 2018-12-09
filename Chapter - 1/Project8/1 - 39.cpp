#include <GL/freeglut.h>
#include <stdlib.h>
#include <random>
#include <stdio.h>
#include <math.h>
#include "load_image.h"
GLvoid drawScene(GLvoid);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void SpecialKeys(int key, int x, int y);
static int mode = 1;


static float cycleRad = 200;


struct point_3 {
   float x = 0;
   float y = 450;
   float z = 0;
};

#define pi 3.14
//#define DEGTORAD pi/360 * 2
#define DEGTORAD pi/180
#define pointArrSize 20
#define ShapeSize 15

static point_3 spherePos;
static BOOL start = FALSE;
static BOOL bCrash = FALSE;
static BOOL goaway = FALSE;
static BOOL vectorOn = FALSE;

GLdouble result[16] = { 0 };
GLdouble Identity[16] = {
   1.0, 0.0, 0.0, 0.0,
   0.0, 1.0, 0.0, 0.0,
   0.0, 0.0, 1.0, 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateX_P3[16] = {
   1.0, 0.0, 0.0, 0.0,
   0.0, cos(3 * DEGTORAD), -sin(3 * DEGTORAD), 0.0,
   0.0, sin(3 * DEGTORAD), cos(3 * DEGTORAD), 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateX_M3[16] = {
   1.0, 0.0, 0.0, 0.0,
   0.0, cos(-3 * DEGTORAD), -sin(-3 * DEGTORAD), 0.0,
   0.0, sin(-3 * DEGTORAD), cos(-3 * DEGTORAD), 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateY_P3[16] = {
   cos(3 * DEGTORAD), 0.0, sin(3 * DEGTORAD), 0.0,
   0.0, 1.0, 0.0, 0.0,
   -sin(3 * DEGTORAD), 0.0, cos(3 * DEGTORAD), 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateY_M3[16] = {
   cos(-3 * DEGTORAD), 0.0, sin(-3 * DEGTORAD), 0.0,
   0.0, 1.0, 0.0, 0.0,
   -sin(-3 * DEGTORAD), 0.0, cos(-3 * DEGTORAD), 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateZ_P3[16] = {
   cos(3 * DEGTORAD), -sin(3 * DEGTORAD), 0.0, 0.0,
   sin(3 * DEGTORAD), cos(3 * DEGTORAD), 0.0, 0.0,
   0.0, 0.0, 1.0, 0.0,
   0.0, 0.0, 0.0, 1.0
};
GLdouble rotateZ_M3[16] = {
   cos(-3 * DEGTORAD), -sin(-3 * DEGTORAD), 0.0, 0.0,
   sin(-3 * DEGTORAD), cos(-3 * DEGTORAD), 0.0, 0.0,
   0.0, 0.0, 1.0, 0.0,
   0.0, 0.0, 0.0, 1.0
};

GLvoid MultiMatrix(GLdouble y[16]) {
   for (int i = 0; i < 16; i++) {
      result[i] = 0;
   }

   for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
         result[j + i * 4] = Identity[0 + i * 4] * y[0 + j] +
            Identity[1 + i * 4] * y[4 + j] +
            Identity[2 + i * 4] * y[8 + j] +
            Identity[3 + i * 4] * y[12 + j];
      }
   }

   for (int i = 0; i < 16; i++) {
      Identity[i] = result[i];
   }
}
GLfloat ctrlpoints[3][3][3] = { { { -200, 0, -200 },{ 0, 0, -200 },{ 200, 0, -200 } },{ { -200, 0, 0 },{ 0,350,0 },{ 200, 0, 0 } },{ { -200,0,200 },{ 0,0,200 },{ 200,0,200 } } };


void reset()
{
   start = FALSE;
   bCrash = FALSE;
   goaway = FALSE;
   cycleRad = 200;
   spherePos.x = 0;
   spherePos.z = 0;
   spherePos.y = 300;
}

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
   Cam() // ������
   {
      m_Eye = { 0.0, 0.0, 0.0 };
      m_Center = { 0.0, 0.0, -100.0 };
      m_Up = { 0.0, 1.0, 0.0 };
      m_MoveEye = { 0.0, 0.0, 0.0 };
      m_Angle = { 0.0, 0.0, 0.0 };
   }
   void Init() // �ʱ�ȭ �Լ�
   {
      m_Eye = { 0.0, 0.0, 0.0 };
      m_Center = { 0.0, 0.0, -100.0 };
      m_Up = { 0.0, 1.0, 0.0 };
      m_MoveEye = { 0.0, 0.0, 0.0 };
      m_Angle = { 0.0, 0.0, 0.0 };
   }

   void drawCamera() // ī�޶� ��ġ�ϴ� �Լ�
   {
      //glPushMatrix();
      //glLoadIdentity(); // ������ ��ȯ ����� ������ķ� �ʱ�ȭ - �ð� ��ǥ�踦 �������� �ʱ�ȭ
      // ���� ��ȯ: ī�޶��� ��ġ ���� (�ʿ��� ���, �ٸ� ���� ���� ����)
      gluLookAt(m_Eye.x, m_Eye.y, m_Eye.z, m_Center.x, m_Center.y, m_Center.z, m_Up.x, m_Up.y, m_Up.z);

      glRotated(-m_Angle.x, 1.0, 0.0, 0.0);
      glRotated(m_Angle.y, 0.0, 1.0, 0.0);
      glRotated(m_Angle.z, 0.0, 0.0, 1.0);
      glTranslated(-m_MoveEye.x, -m_MoveEye.y, m_MoveEye.z);
      //glPopMatrix();
   }

public:
   void rotateEye(double x, double y, double z) // ī�޶� ȸ�� �Լ�
   {
      m_Angle.x += x;
      m_Angle.y += y;
      m_Angle.z += z;
   }
   void moveEye(double x, double y, double z) // ī�޶� �̵� �Լ�
   {
      m_MoveEye.x += x;
      m_MoveEye.y += y;
      m_MoveEye.z += z;
   }
};

static int count = 0;
static Cam camera;
void Mouse(int button, int state, int x, int y);

struct rightPos {
   float x;
   float y;
   float z;

   int rad = 0;
};

static rightPos rp[2];
static rightPos moon;

GLfloat am[] = { -50, 100, 0, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt[] = { 0.6f, 0.5f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.6f, 0.5f, 0.2f, 1.0f };
GLfloat specu[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray[] = { 0.25f, 0.25f, 0.25f, 0.1f };

GLfloat am1[] = { 50.0f, 100.0f, 0.0f, 1.0f };
//GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 0.6f };
GLfloat ambientLignt1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat specu1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray1[] = { 0.25f, 0.25f, 0.25f, 0.1f };


void SetupRC()
{
   camera.rotateEye(0, 0, 0);
   spherePos.y = 300;

   rp[0].y = 50;
   rp[0].rad = 180;
   rp[0].x = 100 * cos(3.141592 / 180.f * rp[0].rad);
   rp[0].z = 100 * sin(3.141592 / 180.f * rp[0].rad);

   am[0] = rp[0].x;
   am[1] = rp[0].y;
   am[2] = rp[0].z;

   rp[1].y = 50;
   rp[1].rad = 0;
   rp[1].x = 100 * cos(3.141592 / 180.f * rp[1].rad);
   rp[1].z = 100 * sin(3.141592 / 180.f * rp[1].rad);

   am1[0] = rp[1].x;
   am1[1] = rp[1].y;
   am1[2] = rp[1].z;

   moon.rad = 0;
   moon.x = 50;
   moon.y = 50;
   moon.z = 0;

   texture[0] = LoadTexture("texture1.bmp", 100, 100);
   texture[1] = LoadTexture("texture2.bmp", 100, 100);
   texture[2] = LoadTexture("texture3.bmp", 100, 100);
   texture[3] = LoadTexture("texture4.bmp", 100, 100);
   texture[4] = LoadTexture("texture5.bmp", 100, 100);
   texture[5] = LoadTexture("texture6.bmp", 100, 100);

}

static BOOL right1 = FALSE;
static BOOL right2 = FALSE;



void main(int agrc, char *argv[]) { // ������ �ʱ�ȭ �� ���� 
   glutInit(&agrc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   // ���� ���۸��� 3�������� �׸���. 
   glutInitWindowSize(800, 600);
   glutCreateWindow("Points Drawing");
   // ���� ���� �ʱ�ȭ �Լ� 
   SetupRC();
   // �ʿ��� �ݹ� �Լ� ����
   glutMouseFunc(Mouse);
   glutDisplayFunc(drawScene); // ��� �ݹ� �Լ� 
   glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ� �Լ�
   glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ� �Լ�
   glutTimerFunc(100, TimerFunction, 1); // Ÿ�̸� �ݹ� �Լ�
   glutSpecialFunc(SpecialKeys);
   glutMainLoop(); // �̺�Ʈ ���� �����ϱ�
}


GLvoid DrawPlane()
{
   glEnable(GL_TEXTURE_2D);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, texture[5]);
   glBegin(GL_QUADS);
   glTexCoord2d(0, 0);
   glVertex3f(-200, 0, -200);

   glTexCoord2d(1, 0);
   glVertex3f(-200, 0, 200);

   glTexCoord2d(1, 1);
   glVertex3f(200, 0, 200);

   glTexCoord2d(0, 1);
   glVertex3f(200, 0, -200);
   glEnd();

   glPopMatrix();
}


GLvoid DrawRight1(float x, float y, float z)
{
   glPushMatrix();
   glTranslatef(x, y, z);
   //glColor3f(1.0f, 0.1f, 0.1f);
   glutSolidSphere(5, 5, 5);
   glPopMatrix();
}

GLvoid DrawRight2(float x, float y, float z)
{
   glPushMatrix();
   glTranslatef(x, y, z);
   //glColor3f(0.0f, 1.0f, 0.9f);
   glutSolidSphere(5, 5, 5);
   glPopMatrix();
}

GLvoid DrawMoon(float x, float y, float z)
{
   glPushMatrix();
   glTranslatef(x, y, z);
   glutSolidSphere(15, 15, 15);
   glPopMatrix();
}

GLvoid drawPyramid(float x, float z)
{
   glEnable(GL_TEXTURE_2D);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_CCW);

   if (vectorOn == FALSE) {
      glEnable(GL_NORMALIZE);
      glEnable(GL_AUTO_NORMAL);
   }

   glPushMatrix();
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_POLYGON);
   glVertex3f(x - 25, 5, z + 25);
   glVertex3f(x - 25, 5, z - 25);
   glVertex3f(x + 25, 5, z - 25);
   glVertex3f(x + 25, 5, z + 25);
   glEnd();
   glPopMatrix();

   glColor3f(1.0f, 1.0f, 1.0f);
   glPushMatrix();

   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glBegin(GL_POLYGON);
   if (vectorOn == FALSE)
      glNormal3f((0 * -50) - (-50 * 45), (0 * -50) - (-50 * 25), (0 * 45) - (0 * 25));

   glTexCoord2d(0, 0);
   glVertex3f(x + 25, 5, z - 25);

   glTexCoord2d(1, 0);
   glVertex3f(x + 25, 5, z + 25);

   glTexCoord2d(0.5, 1);
   glVertex3f(x, 50, z);
   glEnd();


   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glBegin(GL_POLYGON);
   if (vectorOn == FALSE)
      glNormal3f((0 * 25) - (50 * 45), (0 * 25) - (50 * -25), (0 * 45) - (0 * -25));

   glTexCoord2d(0, 0);
   glVertex3f(x - 25, 5, z + 25);

   glTexCoord2d(1, 0);
   glVertex3f(x - 25, 5, z - 25);

   glTexCoord2d(0.5, 1);
   glVertex3f(x, 50, z);
   glEnd();


   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glBegin(GL_POLYGON);
   if (vectorOn == FALSE)
      glNormal3f((0 * 25) - (0 * 45), (-50 * 25) - (0 * -25), (-50 * 45) - (0 * -25));

   glTexCoord2d(0, 0);
   glVertex3f(x + 25, 5, z - 25);

   glTexCoord2d(1, 0);
   glVertex3f(x - 25, 5, z - 25);

   glTexCoord2d(0.5, 1);
   glVertex3f(x, 50, z);
   glEnd();

   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glBegin(GL_POLYGON);
   if (vectorOn == FALSE)
      glNormal3f((0 * -25) - (0 * 45), (50 * -25) - (0 * 25), (50 * 45) - (0 * 25));

   glTexCoord2d(0, 0);
   glVertex3f(x - 25, 5, z + 25);

   glTexCoord2d(1, 0);
   glVertex3f(x + 25, 5, z + 25);

   glTexCoord2d(0.5, 1);
   glVertex3f(x, 50, z);
   glEnd();
   glPopMatrix();
}

static BOOL culling = TRUE;

GLfloat fog_color[4] = { 0.7, 0.0, 0.1, 1.0 };
GLfloat density = 0.7;
GLfloat _start = 100.0;
GLfloat end = 500.0;
int moder = 0;
GLvoid drawScene(GLvoid)
{
   glEnable(GL_DEPTH_TEST);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);

   glPushMatrix();
   camera.drawCamera();
   DrawMoon(moon.x, moon.y, moon.z);
   DrawRight1(am[0], am[1], am[2]);
   DrawRight2(am1[0], am1[1], am1[2]);

   glEnable(GL_FOG);
   {
      switch (moder)
      {
      case 0:
         glFogf(GL_FOG_MODE, GL_LINEAR);
         break;
      case 1:
         glFogf(GL_FOG_MODE, GL_EXP);
         break;
      case 2:
         glFogf(GL_FOG_MODE, GL_EXP2);
         break;
      }

      glFogfv(GL_FOG_COLOR, fog_color);
      glFogf(GL_FOG_START, start);
      glFogf(GL_FOG_END, end);
   }
   glPushMatrix();
   if (right1 == TRUE) {
      //glEnable(GL_AUTO_NORMAL);
      glEnable(GL_LIGHTING);
      //glColor3f(1.0f, 1.0f, 1.0f);
      //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
      //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
      //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
      //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
      //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLignt);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
      glLightfv(GL_LIGHT0, GL_SPECULAR, specu);
      glLightfv(GL_LIGHT0, GL_POSITION, am);

      glEnable(GL_COLOR_MATERIAL);

      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      // ���Ŀ� ������ ��� ������ ��� ������ ���� ���ݻ� �ݻ����� ���´�.
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
      //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLignt);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specu);
      glMateriali(GL_FRONT, GL_SHININESS, 64);

      glEnable(GL_LIGHT0);
   }
   else if (right1 == FALSE)
      glDisable(GL_LIGHT0);

   if (right2 == TRUE) {
      glEnable(GL_LIGHTING);
      //glColor3f(1.0f, 1.0f, 1.0f);
      ////glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
      //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLignt);
      //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
      //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
      //glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLignt1);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
      glLightfv(GL_LIGHT1, GL_SPECULAR, specu1);
      glLightfv(GL_LIGHT1, GL_POSITION, am1);

      glEnable(GL_COLOR_MATERIAL);

      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      // ���Ŀ� ������ ��� ������ ��� ������ ���� ���ݻ� �ݻ����� ���´�.
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray1);
      //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLignt1);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specu1);
      glMateriali(GL_FRONT, GL_SHININESS, 64);

      glEnable(GL_LIGHT1);
   }
   else if (right2 == FALSE)
      glDisable(GL_LIGHT1);
   glPopMatrix();


   DrawPlane();
   drawPyramid(-150, -150);
   drawPyramid(150, 150);
   drawPyramid(150, -150);
   drawPyramid(-150, 150);
   drawPyramid(0, 0);

   glPopMatrix();
   glutSwapBuffers();
   //   glFlush(); // ȭ�鿡 ����ϱ� 
}



static float zZoom = 45;
GLvoid Reshape(int w, int h)
{
   GLfloat nRange = 800.0f;

   glViewport(-1, -1, w, h);  // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.


   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (mode == 1) {
      //-- ������ ���� ���� �Ǵ� ���� ���� �� �� ���� �����Ѵ�. // 1. Ŭ���� ���� ����: ���� ������ ���
      gluPerspective(zZoom, 1.0, 1.0, 1000.0);
      
   }
   else if (mode == 2)
      glOrtho(-400.0, 400.0, -300.0, 300.0, -400.0, 400.0);

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();
   glTranslatef(0.0, 0.0, -500.0);     // ���� ������ ȭ�� �������� �̵��Ͽ� �þ߸� Ȯ���Ѵ�.
}

static int sphereRad = 0;

static BOOL AUTO = FALSE;

void TimerFunction(int value)
{
   glutPostRedisplay();

   if (AUTO == TRUE) {

      moon.rad = (moon.rad + 1) % 360;

      moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
      moon.z = 50 * sin(3.141592 / 180.f * moon.rad);
   }

   glutTimerFunc(1, TimerFunction, 100);
}

void Keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case 'q':
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
   case 'i':
      camera.moveEye(0, 1, 0);
      break;
   case 'j':
      camera.moveEye(-1, 0, 0);
      break;

   case 'k':
      camera.moveEye(0, -1, 0);
      break;

   case 'l':
      camera.moveEye(1, 0, 0);
      break;
   case '+':
      camera.moveEye(0, 0, 1);
      break;
   case '-':
      camera.moveEye(0, 0, -1);
      break;
   case 'o':
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

   case '1':
      if (right1 == FALSE)
         right1 = TRUE;
      else
         right1 = FALSE;
      break;

   case '2':
      if (right2 == FALSE)
         right2 = TRUE;
      else
         right2 = FALSE;
      break;

   case 'r':
   case 'R':
      if (AUTO == FALSE)
         AUTO = TRUE;
      else
         AUTO = FALSE;
      break;

   case 't':
      moon.rad = (moon.rad + 10) % 360;

      moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
      moon.z = 50 * sin(3.141592 / 180.f * moon.rad);

      break;

   case 'T':
      moon.rad -= 10;
      if (moon.rad <= 0)
         moon.rad = 360;

      moon.x = 50 * cos(3.141592 / 180.f * moon.rad);
      moon.z = 50 * sin(3.141592 / 180.f * moon.rad);
      break;
   case 'a':
      //PostQuitMessage(0);
      break;

   case 'd':
      density -= 0.1f;
      break;
   case 's':
      _start -= 5.f;
      break;
   case 'e':
      end -= 5.f;
      break;

   case 'D':
      density += 0.1f;
      break;
   case 'S':
      _start += 5.f;
      break;
   case 'E':
      end += 5.f;
      break;
   case 'm':
      moder += 1;
      moder %= 3;
      break;
   }
}



static int swi = 0;
void Mouse(int button, int state, int x, int y)
{

   glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y) {
   if (key == GLUT_KEY_SHIFT_L)
      start = TRUE;
   glutPostRedisplay();
}