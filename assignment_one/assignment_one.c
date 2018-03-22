#include "includes.h"
#include "keyboard.h"

#define OCEAN_FLOOR -1
//segments will eventually have to be a var not a def in order to facilitate
//wave resolution modification
#define SEGMENTS 100
#define L_MAX -1.0
#define R_MAX 1.0
#define AMP .2
#define WL 1


/*initialize the global flags ther were declared in includes.h*/
bool wave_wire_flag = false;

/*There will need to be a bunch of flags up here to handle game setting toggles
 * on the key board*/
static const float PI = acos(-1.0);
typedef struct {float t, lastT, dt;} Global;

Global g;

void idle()
{
    g.lastT = g.t;
    g.t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    g.dt = g.t - g.lastT; 
    //printf("idle function called\n");
    glutPostRedisplay();
}


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

/*should this take arguments? who knows.*/
void drawOcean() 
{
    /*need to add facilities for lowering resolution/drawing tan/wireframe/
     * drawing ortho*/
    if(wave_wire_flag)
    {
        /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         * THIS MAY AFFECT OTHER POLYS, RESET AT END OF drawOcean()!
         */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glBegin(GL_QUAD_STRIP);
    float k = (2 * PI) / WL; /*effectively PI, open to change*/
    float x = 0; 
    float y = 0;
    float stepSize = (R_MAX - L_MAX)/SEGMENTS;
    glColor4f(0,.7,1.0,.6);
    for(int i = 0; i <= SEGMENTS; i++)
    {
        x = (i * stepSize) + L_MAX;
        y = AMP * sinf((k * x) + ((PI/4.0) * g.t));
        /*printf("%f <- x\n", x);
        printf("%f <- y\n\n", y);*/
        glVertex3f(x, y, 1);
        glVertex3f(x, OCEAN_FLOOR, 1);
    }

    glEnd();        
    //reset poly mode.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return;
}

void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawAxes(1.0);
    drawOcean();

    if((err = glGetError()))
    {
        printf("%s\n", (gluErrorString(err)));
    }
    
    glutSwapBuffers();
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
	glutInitWindowSize(700, 700);
    glutCreateWindow("Island defence 2D");

    init();
    
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}
