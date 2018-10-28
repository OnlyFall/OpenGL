#include "default.h"

GLvoid TimerFunction(int value)
{

	glutPostRedisplay();// 화면 재출력을 위해 디스플레이 콜백 함수 호출.
	glutTimerFunc(10, TimerFunction, 1);
}