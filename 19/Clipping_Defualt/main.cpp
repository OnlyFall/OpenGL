#include "default.h"

bool isCord = true;
Cam cam;

void main(int argc, char** argv)    // 윈도우 출력하고 출력함수 설정
{   // 윈도우 초기화 및 생성
	glutInit(&argc, argv);    // glut 초기화        
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA / GLUT_DEPTH);  // 디스플레이 모드 설정        
	glutInitWindowPosition(100, 100);   // 윈도우의 위치 지정
	glutInitWindowSize(XSize, YSize);    // 윈도우의 크기 지정
	glutCreateWindow("Defualt");   // 윈도우 생성 (윈도우 이름)  
	
	// 상태 변수 초기화 함수
	SetupRC();
	cam.Init_Cam();

	/*<<<<<<콜백 함수>>>>>>*/

	// 출력 함수의 지정
	glutDisplayFunc(drawScene);    
	//마우스 입력 함수 지정
	glutMouseFunc(Mouse);

	//키보드 입력 함수 지정
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	//타이머 함수 지정
	glutTimerFunc(100, TimerFunction, 1);

	// 다시 그리기 함수 지정
	glutReshapeFunc(reshape);       

	// 이벤트 처리 시작 
	glutMainLoop(); 
}

// 출력 함수 
GLvoid drawScene()    
{
	cam.CamApply();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Draw 함수.
	draw();

	glutSwapBuffers(); //버퍼 교환 (결과 출력)
}
GLvoid reshape(int w, int h)    // 다시 그리기 함수
{
	//뷰포트 변환 설정.
	glViewport(0, 0, w, h);

	//투영 행렬 스택 재설정.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//투영 선택 : 원근 투영, 직각 투영.

	//원근 투영
	gluPerspective(60.0f, w / h, 1.0, 600.0);
	//glTranslatef(0.0, 0.0, -300.0); 시점을 뒤로 이동 Translate 방법

	//직각 투영
	//glOrtho(-XSize / 2, XSize / 2, -YSize / 2, XSize / 2, -ZSize / 2, ZSize / 2);


	//모델 뷰 행렬 스택 재설정.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	//관측 변환 : 카메라의 위치 설정
	//gluLookAt(0.0, 0.0, -300.0, 0.0, 0.0, 300.0, 0.0, -1.0, -300.0); 시점을 뒤로 이동 gluLookAt 방법
	//gluLookAt(0.0, 0.0, 300.0, 0.0, 0.0, -300.0, 0.0, 1.0, 300.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void SetupRC()
{

	/*필요한 변수들, 좌표값 등의 초기화.
	기능 설정 초기화.*/
	isCord = true;
	srand(time(NULL));
	spherePos.x = 0;
	spherePos.y = 0;
	spherePos.z = 0;
	for (int i = 0; i < 16; i++) {
		if (i % 5 == 0)
			Identity[i] = 1;
		else
			Identity[i] = 0;

	}
}