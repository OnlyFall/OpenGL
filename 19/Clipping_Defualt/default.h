#pragma once
#include <GL/freeglut.h>     // 또는 <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
//#include <vector>//나중에 적용하세용^^
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

//bool 변수.
//* <- 다시 시작 할때, 초기화 시켜줘야 하는 항목.
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

/* 입력 함수 선언*/
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

/*출력 함수*/
GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);
GLvoid draw();

/*Easy 함수*/
GLvoid leftClick(int x, int y);
GLvoid rightClick(int x, int y);

/*Draw 함수 선언*/
GLvoid DrawCoordinate(); // 좌표축
GLvoid DrawCubeSample(int Size);//Cube Sample
GLvoid MultiMatrix(GLdouble y[16]);

/* 초기화 */
void SetupRC();

