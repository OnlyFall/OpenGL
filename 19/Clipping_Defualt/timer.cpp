#include "default.h"

GLvoid TimerFunction(int value)
{

	glutPostRedisplay();// ȭ�� ������� ���� ���÷��� �ݹ� �Լ� ȣ��.
	glutTimerFunc(10, TimerFunction, 1);
}