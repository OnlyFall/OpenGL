#include <gl/freeglut.h> 

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);


void main(int argc, char *argv[])
{
	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// ���÷��� ��� ���� 
	glutInitWindowPosition(100, 100);
	// �������� ��ġ���� 
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ���� 
	glutReshapeFunc(Reshape); glutMainLoop();
} // ������ ��� �Լ� 


GLvoid drawScene(GLvoid)
{
	GLint p1[2] = { 0, 1.0f }; 
	GLint p2[2] = { 0, -1.0f }; 
	GLint p3[2] = { -1.0f, 0 };
	GLint p4[2] = {1.0f, 0};

	GLint tr1[2] = { 0, 0.1f};
	GLint tr2[2] = { -0.1f , -0.1f };
	GLint tr3[2] = { 0.1f, -0.1f };
	
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // �������� 'blue' �� ���� 
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ� 
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // �׸��� ���� 'green' ���� ����
	glRectf(-1.0f, 1.0f, -0.7f, 0.7f); // �簢�� 
	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);// ���
	glRectf(1.0f, 1.0f, 0.7f, 0.7f); // �簢�� �׸��� 
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW
	glRectf(-1.0f, -1.0f, -0.7f, -0.7f); // �簢�� �׸��� 
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);//RED
	glRectf(1.0f, -1.0f, 0.7f, -0.7f); // �簢�� �׸��� 
	
	glBegin(GL_LINES); 
	glVertex2iv(p1);
	glVertex2iv(p2); 
	glVertex2iv(p3); 
	glVertex2iv(p4);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);//�ϴ�
	glVertex2d(0, 0.1f);
	glVertex2d(-0.1f, -0.1f);
	glVertex2d(0.1f, -0.1f);
	glEnd();

//	glOrtho(-400.0, 400.0, -350.0, 350.0, -1.0, 1.0);


	glFlush(); // ȭ�鿡 ����ϱ� 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	//glOrtho(-400.0, 400.0, -300.0, 300.0, -1.0, 1.0);
}
			