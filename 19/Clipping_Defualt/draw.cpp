#include "default.h"
GLvoid draw();
GLvoid DrawSphere();
GLvoid DrawPlane(int Size);
//��ȯ + �׸��� ����.
	/*
	1. glPushMatrix �Լ� ȣ��� '������ ��ǥ �ý����� ����.'
	2. �ʿ��� ��� glLoadIdentity();�� '��� �ʱ�ȭ.'
	3. '��ȯ �Լ� ȣ��' (�̵�, ȸ��, ����)
	4. ��ȯ�� ���� �Ŀ��� ������ ��ǥ �ý����� �ٽ� �����ϱ� ���ؼ� glPopMatrix(); �Լ� ȣ��.
	5. �ʿ��� �׸��� �۾� ����.
	*/
point_3 spherePos;
point_3 sphereAngle;

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

//main Draw
GLvoid draw() {

	DrawSphere();
	DrawPlane(100);
	DrawCoordinate();
	//DrawCubeSample(50);
}

GLvoid DrawSphere() {
	glPushMatrix();
	
	//Move
	glTranslatef(spherePos.x, spherePos.y, spherePos.z);
	//Rotation ��� �����ֱ�.
	glMultMatrixd(Identity);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutWireSphere(20, 20, 20);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutSolidSphere(20, 10, 10);
	glPopMatrix();
}

GLvoid DrawPlane(int Size) {
	//XZ
	glPushMatrix();
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3i(Size, -20, Size);
	glVertex3i(Size, -20, -Size);
	glVertex3i(-Size, -20, -Size);
	glVertex3i(-Size, -20, Size);
	glEnd();
	glPopMatrix();
	//XY
	/*glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3i( Size, Size, - Size);
	glVertex3i(-Size, Size, - Size);
	glVertex3i(-Size, -20, - Size);
	glVertex3i(Size, -20, - Size);
	glEnd();
	glPopMatrix();*/
}
//Default Draw
GLvoid DrawCoordinate()//F1 Ű �Է½� ��ǥ�� ����.
{
	if (isCord) {
		glPushMatrix();
		{
			glBegin(GL_LINES);
			//X�� (RED)
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glVertex3i(-XSize / 4, 0, 0);
			glVertex3i(XSize / 4, 0, 0);
			//Y�� (GREEN)
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			glVertex3i(0, -YSize / 4, 0);
			glVertex3i(0, YSize / 4, 0);
			//Z�� (BLUE)
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glVertex3i(0, 0, -ZSize / 4);
			glVertex3i(0, 0, ZSize / 4);
			glEnd();
		}
		glPopMatrix();
	}
}

GLvoid DrawCubeSample(int Size) {
	glPushMatrix();
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glutSolidCube(Size);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutWireCube(Size);
	glPopMatrix();
}

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