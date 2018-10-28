#pragma once
#include <GL/freeglut.h>     // �Ǵ� <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//#include <vector>//���߿� �����ϼ���^^
#include <iostream>
#include "cam.h"

#define Min(x, y) (((x) > (y)) ? (x) : (y))

#define XSize 800
#define YSize 600
#define ZSize 600

#define ShapeSize 50

#define pi 3.14
#define DEGTORAD pi/360 * 2

#define pointArrSize 2

struct point_3 {
	float x = 0;
	float y = 0;
	float z = 0;
};

//bool ����.
//* <- �ٽ� ���� �Ҷ�, �ʱ�ȭ ������� �ϴ� �׸�.
extern bool isCord;
extern Cam cam;
// For Sphere Rotation
extern GLdouble result[16];
extern GLdouble Identity[16];
extern GLdouble rotateX_P3[16];
extern GLdouble rotateX_M3[16];
extern GLdouble rotateY_P3[16];
extern GLdouble rotateY_M3[16];
extern GLdouble rotateZ_P3[16];
extern GLdouble rotateZ_M3[16];

extern point_3 spherePos;
extern point_3 sphereAngle;

/* �Է� �Լ� ����*/
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

/*��� �Լ�*/
GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid draw();

/*Easy �Լ�*/
GLvoid leftClick(int x, int y);
GLvoid rightClick(int x, int y);

/*Draw �Լ� ����*/
GLvoid DrawCoordinate(); // ��ǥ��
GLvoid DrawCubeSample(int Size);//Cube Sample
GLvoid MultiMatrix(GLdouble y[16]);

/* �ʱ�ȭ */
void SetupRC();

