#include <gl/freeglut.h>
#include <stdio.h>
#include <stdlib.h>


GLuint texture[7];
GLubyte *pBytes;
BITMAPINFO *info;

GLuint LoadTexture(const char * filename, int width_1, int height_1)
{
	GLuint texture;
	int width, height;
	unsigned char * data;
	FILE * file;

	// ���� ����
	fopen_s(&file, filename, "rb");

	if (file == NULL) return 0;
	width = width_1;
	height = height_1;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// �������
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	// ���� PDF����
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Modulate�� ��
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	// ���� �Ӹ����� ����

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}
