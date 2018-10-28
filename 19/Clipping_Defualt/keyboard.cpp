#include "default.h"

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'i':
		SetupRC();
		break;
	case 'y':
		MultiMatrix(rotateY_P3);
		break;
	case 'Y':
		MultiMatrix(rotateY_M3);
		break;
	}
	cam.rotate_key(key);
	cam.move_key(key);
	cam.zoom_key(key);

	cam.Debug_Log();
	glutPostRedisplay();// ȭ�� ������� ���� ���÷��� �ݹ� �Լ� ȣ��.
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
		isCord = !isCord;
	switch (key) {
		//Sphere Control

	case GLUT_KEY_UP:
		//Move
		if(spherePos.z > -100)
			spherePos.z -= 1;
		//ȸ�� ��Ŀ� x�� 3�� ȸ�� ��� ���ϱ�.
		MultiMatrix(rotateX_P3);
		break;
	case GLUT_KEY_DOWN:
		if(spherePos.z < 100)
			spherePos.z += 1;
		//sphereAngle.x += 3;
		MultiMatrix(rotateX_M3);
		break;
	case GLUT_KEY_LEFT:
		if(spherePos.x > -100)
			spherePos.x -= 1;
		//sphereAngle.z += 3;
		MultiMatrix(rotateZ_M3);

		break;
	case GLUT_KEY_RIGHT:
		if(spherePos.x < 100)
			spherePos.x += 1;
		//sphereAngle.z -= 3;
		MultiMatrix(rotateZ_P3);

		break;
	}
	
	glutPostRedisplay();
}