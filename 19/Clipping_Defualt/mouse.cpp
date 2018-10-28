#include "default.h"



GLvoid Mouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		leftClick(x, y);
	}
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
	{
		rightClick(x, y);
	}
}
//Easy function
GLvoid leftClick(int x, int y) {
	
}

GLvoid rightClick(int x, int y) {

}