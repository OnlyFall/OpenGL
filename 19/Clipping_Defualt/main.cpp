#include "default.h"

bool isCord = true;
Cam cam;

void main(int argc, char** argv)    // ������ ����ϰ� ����Լ� ����
{   // ������ �ʱ�ȭ �� ����
	glutInit(&argc, argv);    // glut �ʱ�ȭ        
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA / GLUT_DEPTH);  // ���÷��� ��� ����        
	glutInitWindowPosition(100, 100);   // �������� ��ġ ����
	glutInitWindowSize(XSize, YSize);    // �������� ũ�� ����
	glutCreateWindow("Defualt");   // ������ ���� (������ �̸�)  
	
	// ���� ���� �ʱ�ȭ �Լ�
	SetupRC();
	cam.Init_Cam();

	/*<<<<<<�ݹ� �Լ�>>>>>>*/

	// ��� �Լ��� ����
	glutDisplayFunc(drawScene);    
	//���콺 �Է� �Լ� ����
	glutMouseFunc(Mouse);

	//Ű���� �Է� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	//Ÿ�̸� �Լ� ����
	glutTimerFunc(100, TimerFunction, 1);

	// �ٽ� �׸��� �Լ� ����
	glutReshapeFunc(reshape);       

	// �̺�Ʈ ó�� ���� 
	glutMainLoop(); 
}

// ��� �Լ� 
GLvoid drawScene()    
{
	cam.CamApply();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Draw �Լ�.
	draw();

	glutSwapBuffers(); //���� ��ȯ (��� ���)
}
GLvoid reshape(int w, int h)    // �ٽ� �׸��� �Լ�
{
	//����Ʈ ��ȯ ����.
	glViewport(0, 0, w, h);

	//���� ��� ���� �缳��.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//���� ���� : ���� ����, ���� ����.

	//���� ����
	gluPerspective(60.0f, w / h, 1.0, 600.0);
	//glTranslatef(0.0, 0.0, -300.0); ������ �ڷ� �̵� Translate ���

	//���� ����
	//glOrtho(-XSize / 2, XSize / 2, -YSize / 2, XSize / 2, -ZSize / 2, ZSize / 2);


	//�� �� ��� ���� �缳��.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	//���� ��ȯ : ī�޶��� ��ġ ����
	//gluLookAt(0.0, 0.0, -300.0, 0.0, 0.0, 300.0, 0.0, -1.0, -300.0); ������ �ڷ� �̵� gluLookAt ���
	//gluLookAt(0.0, 0.0, 300.0, 0.0, 0.0, -300.0, 0.0, 1.0, 300.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void SetupRC()
{

	/*�ʿ��� ������, ��ǥ�� ���� �ʱ�ȭ.
	��� ���� �ʱ�ȭ.*/
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