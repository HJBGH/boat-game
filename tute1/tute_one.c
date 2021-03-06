#include <stdlib.h>
#include <stdio.h>

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

void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_QUADS | GL_POINTS);
    /* Put drawing code here */
    glColor3ui(0, 0xffffffff, 0);
    glVertex3f(-0.25, -0.25, -0.75);
    glVertex3f(0.75, -0.25, -0.75);
    glVertex3f(0.75, 0.75, -0.75);
    glVertex3f(-0.25, 0.75, -0.75);
    printf("display\n"); 
    glColor3ui(0xffffffff,0,0);
    //glVertex3f(0.0,0.0,0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);   
    glVertex3f(-0.5, 0.5, -0.5);   


    glEnd();

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
    glutCreateWindow("Tutorial 1");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}
