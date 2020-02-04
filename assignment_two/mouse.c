#include "mouse.h"


/*Functions for dealing with mouse movement, this will probably change as 
 * the project progresses*/
void mouseMotion(int x, int y)
{
    C.x += C.lastx - x;
    C.lastx = x;
    C.y += C.lasty - y;
    C.lasty = y;

    //printf("C x %f, C y %f\n", C.x, C.y);
    glutPostRedisplay();
}

void mouseClicked(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        C.lastx = x;
        C.lasty = y;
    }
}

