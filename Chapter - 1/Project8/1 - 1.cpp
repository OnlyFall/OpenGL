#include <gl/freeglut.h> 

GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);


void main(int argc, char *argv[])
{
	glutInit(&argc, argv); glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100);
	// 윈도우의 위치지정 
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정 
	glutReshapeFunc(Reshape); glutMainLoop();
} // 윈도우 출력 함수 


GLvoid drawScene(GLvoid)
{
	GLint p1[2] = { 0, 1.0f }; 
	GLint p2[2] = { 0, -1.0f }; 
	GLint p3[2] = { -1.0f, 0 };
	GLint p4[2] = {1.0f, 0};

	GLint tr1[2] = { 0, 0.1f};
	GLint tr2[2] = { -0.1f , -0.1f };
	GLint tr3[2] = { 0.1f, -0.1f };
	
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // 바탕색을 'blue' 로 지정 
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기 
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // 그리기 색을 'green' 으로 지정
	glRectf(-1.0f, 1.0f, -0.7f, 0.7f); // 사각형 
	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);// 흰색
	glRectf(1.0f, 1.0f, 0.7f, 0.7f); // 사각형 그리기 
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);//YELLOW
	glRectf(-1.0f, -1.0f, -0.7f, -0.7f); // 사각형 그리기 
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);//RED
	glRectf(1.0f, -1.0f, 0.7f, -0.7f); // 사각형 그리기 
	
	glBegin(GL_LINES); 
	glVertex2iv(p1);
	glVertex2iv(p2); 
	glVertex2iv(p3); 
	glVertex2iv(p4);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);//하늘
	glVertex2d(0, 0.1f);
	glVertex2d(-0.1f, -0.1f);
	glVertex2d(0.1f, -0.1f);
	glEnd();

//	glOrtho(-400.0, 400.0, -350.0, 350.0, -1.0, 1.0);


	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	//glOrtho(-400.0, 400.0, -300.0, 300.0, -1.0, 1.0);
}
			