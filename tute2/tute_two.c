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

#define SEGMENTS 100
#define L_MAX -1.0
#define R_MAX 1.0


void drawAxes(float l);

void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBegin(GL_LINE_STRIP);   
    float x = 0;
    float y = 0;
    float stepSize = (R_MAX - L_MAX)/SEGMENTS;
    glColor3f(1.0,1.0,1.0);
    for(int i = 0; i <= SEGMENTS; i++)
    {
        x = (i * stepSize) + L_MAX;
        y = (-2 * powf(x, 2)) + (2 * x) + 1;
        printf("%f <- x\n", x);
        printf("%f <- y\n\n", y);
        glVertex3f(x, y, 0); 
    }
    glEnd();        
    
    drawAxes(1.0);
    
   

    if((err = glGetError()))
    {
        printf("%s\n", (gluErrorString(err)));
    }
    
    glutSwapBuffers();
}

void drawAxes(float l)
{ 
    glBegin(GL_LINES);
    glColor3ui(0, 0xffffffff, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, l, 0);

    glColor3ui(0xffffffff,0,0);
    glVertex3f(0, 0, 0);
    glVertex3f(l, 0, 0);

    glColor3ui(0 ,0, 0xffffffff);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, l);
    glEnd();
    return;
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
