#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#if _WIN32
#	include <Windows.h>
#endif
#if __APPLE__
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#	include <GLUT/glut.h>
#else 
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

/*There will need to be a bunch of flags up here to handle game setting toggles
 * on the key board*/

void drawAxes(float l)
{
	glBegin(GL_LINES);
	
	glColor3f(1.0,0,0);
	glVertex3f(l,0,0);
	glVertex3f(0,0,0);
	glColor3f(0,1.0,0);
	glVertex3f(0,l,0);
	glVertex3f(0,0,0);
	/*It's a bit pointless to draw the z axis now, but I'm sure it'll come
	 * in handy in the next assignment*/
	glColor3f(0,0,1.0);
	glVertex3f(0,0,l);
	glVertex3f(0,0,0);

	glEnd();
}

void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	drawAxes(1.0);

    if((err = glGetError()))
    {
        printf("%s\n", (gluErrorString(err)));
    }
    
    glutSwapBuffers();
}

/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
    case 'q':
        exit(EXIT_SUCCESS); 
		break;
	case ' ':
		printf("Spacebar pressed\n");
		break;
	case 'e':
		break;
    default:
        break;
    }
}

void init()
{
    /* In this program these OpenGL calls only need to be done once,
      but normally they would go elsewhere, e.g. display */

    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
    glutCreateWindow("Island defence 2D");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}
