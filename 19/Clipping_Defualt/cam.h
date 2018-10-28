#pragma once

class Cam {
public:
	void Init_Cam();
	void CamApply();
	void rotate_key(unsigned char input);
	void rotateUpdate();
	void move_key(unsigned char input);
	void zoom_key(unsigned char input);
	void Init_vector();
	void Init_angle();
	void Debug_Log();

private:
	float eyeX;
	float eyeY;
	float eyeZ;
	float atX;//pdf������ center
	float atY;
	float atZ;
	float upX;
	float upY;
	float upZ;
	float range;

	float posX;//at �� eye�� �߰� ����.
	float posY;
	float posZ;

	double Angle_X;
	double Angle_Y;
	double Angle_Z;

	float zoomAngle;
};