#ifdef __APPLE__ // 若作業系統為macOS系統，則使用下列header檔案
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void init();
void display();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(300, 300);

	glutCreateWindow("OpenGL 3D View");

	init();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 5, 15);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0, 0);
	glutWireTeapot(3);

	glFlush();
}