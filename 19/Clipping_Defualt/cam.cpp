#include "default.h"
#include "cam.h"

void Cam::Init_Cam() {
	Init_vector();
	Init_angle();
}

void Cam::Init_vector() {
	//Center
	posX = 0;
	posY = 0;
	posZ = 0;
	//Eye
	eyeX = 0.0;
	eyeY = 0.0;
	eyeZ = 1.0;
	//At
	atX = 0.0;
	atY = 0.0;
	atZ = -1.0;
	//Up
	upX = 0.0;
	upY = 1.0;
	upZ = 1.0;
	//range
	range = 300;
	//Zoom
	zoomAngle = 60.0f;
}
void Cam::Init_angle() {
	Angle_X = 0.0f;
	Angle_Y = 0.0f;
	Angle_Z = 0.0f;
}

void Cam::CamApply() {
	//up은 말그대로 위쪽을 나타냄으로 조정x
	//일단 이동만 만들기에 임시
	eyeX = posX;//x축은 1직선 상에서만 봄 임시.
	atX = posX;
	//upX = posX;

	eyeY = posY;
	atY = posY;//y축은 다 같지
	upY = 1;//

	eyeZ = posZ + range; //멀리 보자 친구양
	atZ = posZ - range;
	
	//모델 뷰 행렬 스택 재설정.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 0.0, 300.0, 0.0, 0.0, -300.0, 0.0, 1.0, 300.0);
	gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, upX, upY, upZ);
}

void Cam::rotateUpdate() {
	/* 일단, X축 회전 */
	eyeZ = cos(Angle_X *DEGTORAD);
	eyeY = sin(Angle_X *DEGTORAD);
	atZ = -cos(Angle_X *DEGTORAD);
	atY = -sin(Angle_X *DEGTORAD);
}

void Cam::rotate_key(unsigned char input) {
	switch (input) {
		//x축 회전
	case 'x':
		Angle_X += 10;
		break;
	case 'X':
		Angle_X -= 10;
		break;
		//y축 회전 
	case 'y':
		Angle_Y += 10;
		break;
	case 'Y':
		Angle_Y -= 10;
		break;
		//z축 회전
	case 'z':
		Angle_Z += 10;
		break;
	case 'Z':
		Angle_Z -= 10;
		break;
	}
}

void Cam::move_key(unsigned char input) {
	switch (input) {
		//x축 이동
	case 'd':
		posX += 5;
		break;
	case 'a':
		posX -= 5;
		break;
		//y축 이동
	case 'w':
		posY += 5;
		break;
	case 's':
		posY -= 5;
		break;
		//z축 이동
	case 'e':
		posZ += 5;
		break;
	case 'q':
		posZ -= 5;
		break;
	}
}

void Cam::zoom_key(unsigned char input) {
	if (input == '+' || input == '-') {
		switch (input) {
		case '+':
			if (zoomAngle < 10)
				return;
			zoomAngle--;
			break;
		case '-':
			if (zoomAngle > 170)
				return;
			zoomAngle++;
			break;
		}

		//투영 행렬 스택 재설정.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//원근 투영
		gluPerspective(zoomAngle, XSize / YSize, 1.0, 600.0);
	}
}

void Cam::Debug_Log() {
	std::cout << "Eye : " << eyeX << " " << eyeY << " " << eyeZ << std::endl;
	std::cout << "Up : " << upX << " " << upY << " " << upZ << std::endl;
	std::cout << "At : " << atX << " " << atY << " " << atZ << std::endl;
	std::cout << "Zomm : " << zoomAngle << std::endl << std::endl;
}