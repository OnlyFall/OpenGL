#include <GL/glut.h>     // 또는 <GL/feeeglut.h>
#include <time.h>
#include <stdlib.h>
#define RectSize 30
#define WindowSize 90

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);
void main(int argc, char** argv)    // 윈도우 출력하고 출력함수 설정
{   // 초기화 함수들 
	glutInit(&argc, argv);    // glut 초기화        
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  // 디스플레이 모드 설정        
	glutInitWindowPosition(100, 100);   // 윈도우의 위치 지정        
	glutInitWindowSize(250, 250);    // 윈도우의 크기 지정       
	glutCreateWindow("Example1");   // 윈도우 생성 (윈도우 이름)         
	glutDisplayFunc(drawScene);    // 출력 함수의 지정         
	glutReshapeFunc(reshape);   // 다시 그리기 함수 지정        
	srand(time(NULL));
	glutMainLoop();  // 이벤트 처리 시작 

}
GLvoid drawScene()     // 출력 함수 
{
	int temp = 0;
	int first = 0;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	int r = 200;
	for (int i = 1; i < 3; i++)
	{
		glColor4f((float)r / 255, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex2d(0, RectSize*i); glVertex2d(WindowSize, RectSize*i);
		glEnd();
		glBegin(GL_LINES);
		glVertex2d(RectSize*i, 0); glVertex2d(RectSize*i, WindowSize);
		glEnd();
	}
	int Vertex = (rand() % 5) + 2;
	for (int j = 0; j < (WindowSize / RectSize); j++)
	{
		for (int i = 0; i < (WindowSize / RectSize); i++)
		{
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			switch (Vertex)
			{
			case 2:
			{
				glBegin(GL_LINES);
				glVertex2d((RectSize*i) + 10, (RectSize*j) + 15);
				glVertex2d((RectSize*i) + 20, (RectSize*j) + 15);
				glEnd();
				temp = 2;
				Vertex++;
			}
			break;
			case 3:
				glBegin(GL_TRIANGLES);
				glVertex2d((RectSize*i) + 15, (RectSize*j) + 10);
				glVertex2d((RectSize*i) + 10, (RectSize*j) + 20);
				glVertex2d((RectSize*i) + 20, (RectSize*j) + 20);
				glEnd();
				if (temp == 2)
					Vertex++;
				else if (temp == 4)
					Vertex--;
				else if (first == 0) {
					first++;
					Vertex++;
					temp = 3;
				}
				temp = 3;
				break;
			case 4:
				glBegin(GL_POLYGON);
				glVertex2d((RectSize*i) + 7.5, (RectSize*j) + 7.5);
				glVertex2d((RectSize*i) + 7.5, (RectSize*j) + 22.5);
				glVertex2d((RectSize*i) + 22.5, (RectSize*j) + 22.5);
				glVertex2d((RectSize*i) + 22.5, (RectSize*j) + 7.5);
				glEnd();
				if (temp == 3)
					Vertex++;
				else if (temp == 5)
					Vertex--;
				else if (first == 0) {
					first++;
					Vertex++;
				}
				temp = 4;
				break;
			case 5:
				glBegin(GL_POLYGON);
				glVertex2d((RectSize*i) + 10, (RectSize*j) + 10);
				glVertex2d((RectSize*i) + 5, (RectSize*j) + 15);
				glVertex2d((RectSize*i) + 10, (RectSize*j) + 20);
				glVertex2d((RectSize*i) + 20, (RectSize*j) + 15);
				glVertex2d((RectSize*i) + 20, (RectSize*j) + 10);
				glEnd();
				Vertex--;
				temp = 5;
				break;
			}
		}
	}
	glFlush();      // 화면에 출력하기


}
GLvoid reshape(int w, int h)    // 다시 그리기 함수
{
	glViewport(-1, -1, w, h);
	glOrtho(0, WindowSize, WindowSize, 0, 0, 1);
}