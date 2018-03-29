#include "includes.h"
#include "keyboard.h"
#include "boats.h"
#include "island.h"
#include "projectiles.h"

#define BOAT_GUN_S 9 /*gun rotation speed*/
#define BOAT_SCALE .1
#define BOAT_SPEED .3 /*slow boats*/
#define BOAT_HP 10
#define TAS_HP 100
#define SHELL_S .2 /*muzzle velocity of projectiles*/

/*initialize the global flags that were declared in includes.h*/
bool wave_wire_flag = false;
bool wave_norm_flag = false;
bool wave_tang_flag = false;
int segments = 64;

Proj2Vec2f *mag[MAG_DEPTH];

Global g;

Island tasmania = 
{
	.hp = TAS_HP,
	.gun_elev = 30,
	.gun_rot_s = 16
};

Boat leftBoat = 
{
	.left = true,
	.hp = BOAT_HP,
	.x = -.5,
	.gun_elev = 30,
	.colors = {1, 0, 0},
    .s = BOAT_SPEED,
	.gun_rot_s = BOAT_GUN_S
};


Boat rightBoat = 
{
	.left = false,
	.hp = BOAT_HP,
	.x = .5,
	.gun_elev = 30,
	.colors = {0, 0, 1},
    .s = BOAT_SPEED,
	.gun_rot_s = BOAT_GUN_S
};

void idle()
{
	/*
	 * I should be updating cooldowns in here*/
    g.lastT = g.t;
    g.t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    g.dt = g.t - g.lastT; 
    //printf("idle function called\n");
	if(tasmania.cd > 0)
	{
		/*this is not a robust way of doing things*/
		tasmania.cd -= g.dt;
		if(tasmania.cd < 0)
		{
			tasmania.cd = 0;
		}
	}	
	if(tasmania.cd == 0 && tasmania.shellp == NULL)
	{
		/* find a free projectile */
		/*this is somewhat precarious, if no free projectile
		 * can be found there'll be problems.
		 * I'm also not 100 percent sure that I should be handling cooldown
		 * behaviour in here
		 * */
		for(int i = 0; i < MAG_DEPTH; i++)
		{
			if((*mag[i]).loaded == false && (*mag[i]).fired == false)
			{
				mag[i]->loaded = true;
				tasmania.shellp = mag[i];
				printf("new shell loaded into island cannon\n");
				break;
			}
		}
		(tasmania.shellp)->p.x = ISLAND_GUN_L * 
								cosf((M_PI * tasmania.gun_elev) / 180);
								/*I HATE RADIANS*/
		/*muzzle x co-ord*/	
		printf("Tasmania gun elev -> %f\n", (tasmania.gun_elev));
		printf("I don't even know %f\n", (tasmania.shellp)->p.x);
		(tasmania.shellp)->p.y = 
						ISLAND_GUN_L * sinf((M_PI * tasmania.gun_elev) / 180) 
									+ HEIGHT_OVER_X; 
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		(tasmania.shellp)->d.x = SHELL_S * cosf((M_PI * tasmania.gun_elev)/180);
		(tasmania.shellp)->d.y = SHELL_S * sinf((M_PI * tasmania.gun_elev)/180);
	}
	/*reload boat shells*/
	/*update shell info, i.e. p co-ords change according to d co-ords*/
    glutPostRedisplay();
}
/*draw a vector with it's origin at x,y to <a,b> scaled by s and normalized
 * if n is true. Color this vector according to cr, cg, and cb*/
void drawVector(float x, float y, float a, float b, float s, bool n, float cr, float cg, float cb)
{
	if(n)
	{
		float c = fabs(sqrt((a*a)+(b*b)));
		a /= c;
		b /= c;
	}

    glColor3f(cr,cg,cb);
    glBegin(GL_LINES);
  	 
    glVertex3f(x, y, 0);
    glVertex3f(x+(a * s), y+(b * s), 0);
    glEnd();
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
	/*variable declaration*/
	float k = (2 * M_PI) / WL; /*effectively PI, open to change*/
    float x = 0; 
    float y = 0;
	//perhaps make stepsize global, we're going to need it for drawing boats
    float stepSize = (R_MAX - L_MAX)/segments;

	/*unfortunately we have to use two loops for this, otherwise
	 *the vectors get draw into the quad strip*/
	if(wave_tang_flag || wave_norm_flag)
    {
        //This is incredibly clunky
        float dy;
        for(int i = 0; i <= segments; i++)
        {
            x = (i * stepSize) + L_MAX;
            y = AMP * sinf((k * x) + ((M_PI/4.0) * g.t));
            /*printf("%f <- x\n", x);*/
            dy = (k * AMP) * cosf((k * x) + ((M_PI/4.0) * g.t));

            //printf("%f <- dy\n\n", dy);
			if(wave_tang_flag)
			{
            	drawVector(x, y, 1, dy, .1, true, 1.0, 0.0, 0.0);         
			}

			if(wave_norm_flag)
			{
				drawVector(x,y,-dy, 1, .1, true, 0.0, 1.0, 0.0);
			}
        }
    }

    if(wave_wire_flag)
    {
        /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         * THIS MAY AFFECT OTHER POLYS, RESET AT END OF drawOcean()!
         */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glBegin(GL_QUAD_STRIP);
   	glColor4f(0,.7,1.0,.45);
    for(int i = 0; i <= segments; i++)
    {
        x = (i * stepSize) + L_MAX;
        y = AMP * sinf((k * x) + ((M_PI/4.0) * g.t));
        /*printf("%f <- x\n", x);
        printf("%f <- y\n\n", y);*/
        glVertex3f(x, y, 1.0);
        glVertex3f(x, OCEAN_FLOOR, 1.0);
    }
    glEnd();
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //reset poly mode.

    return;
}

void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /*Do game over testing before we draw anything*/
	drawAxes(1.0);
	drawIsland(&tasmania);
	drawBoat(&leftBoat, BOAT_SCALE);
	drawBoat(&rightBoat, BOAT_SCALE);
    drawOcean();
	/*draw trajectories, draw projectiles, loop through mag*/
	for(int i = 0; i < MAG_DEPTH; i++)
	{
		if((*mag[i]).loaded == true)
		{
			drawTraj((mag[i]));
		}
	}

	/*iterate through mag, draw each shell and their trajectory 
	 * if they're available*/

    if((err = glGetError()))
    {
        printf("%s: %x\n", (gluErrorString(err)), err);
    }
    
    glutSwapBuffers();
}

void init()
{
    /* In this program these OpenGL calls only need to be done once,
      but normally they would go elsewhere, e.g. display */

	/*malloc projectile array in here*/
	int i;
	for(i = 0; i < MAG_DEPTH; i++)
	{
		mag[i] = (Proj2Vec2f*) malloc(sizeof(Proj2Vec2f*));
		printf("Projectile memory allocated\n");
	}
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
    
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}
