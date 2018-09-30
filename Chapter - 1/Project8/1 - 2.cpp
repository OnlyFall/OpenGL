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
	glutReshapeFunc(Reshape);
	glutMainLoop();
} // 윈도우 출력 함수 

struct RGB {
	float r;
	float g;
	float b;
};

GLvoid drawScene(GLvoid)
{
	//glOrtho(-400.0, 400.0, -350.0, 350.0, -1.0, 1.0);
	GLfloat R = 1.0f;
	GLfloat G = 1.0f;
	GLfloat B = 1.0f;
	float r;
	float g;
	float b;

	int size = rand() % 10 + 1;
	r = rand() % 150 + 100;
	g = rand() % 150 + 100;
	b = rand() % 150 + 100;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < 100; ++j) {
			if(i == 0 || i == 5)
				glColor4f((float)r / 255, 0.0f, 0.0f, 1.0f);
			else if(i == 1 || i == 6)
				glColor4f((float)r / 255, (float)g / 255, 0.0f, 1.0f);
			else if(i == 2 || i == 7)
				glColor4f((float)r / 255, (float)g / 255, (float)b / 255, 0.0f);
			else if(i == 3 || i == 8)
				glColor4f((float)r / 255 , 0.0f, (float)b / 255, 1.0f);
			else if(i == 4 || i == 9)
				glColor4f(0.0f, (float)g / 255, (float)b / 255, 1.0f);
			//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glRectf(i * (800 / size), j * 6, (i + 1) * (800 / size), (j + 1) * 6);

			if (i == 0 || i == 5)
				r -= 1;
			else if (i == 1 || i == 6) {
				r -= 1;
				g -= 1;
			}
			else if (i == 2 || i == 7) {
				r -= 1;
				g -= 1;
				b -= 1;
			}
			else if (i == 3 || i == 8) {
				r -= 1;
				b -= 1;
			}
			else if (i == 4 || i == 9) {
				g -= 1;
				b -= 1;
			}
		} 
		r = rand() % 150 + 100;
		g = rand() % 150 + 100;
		b = rand() % 150 + 100;
	}

	glFlush(); // 화면에 출력하기 
}

GLvoid Reshape(int w, int h)
{
	glViewport(-1, -1, w, h);
	glOrtho(0, 800.0, 600.0, 0, -1.0, 1.0);
}
