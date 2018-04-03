#include "includes.h"
#include "keyboard.h"
#include "boats.h"
#include "island.h"
#include "projectiles.h"

#define BOAT_GUN_S 9 /*gun rotation speed*/
#define BOAT_SPEED .3 /*slow boats*/
#define BOAT_HP 10
#define TAS_HP 100

/*initialize the global flags that were declared in includes.h*/
bool wave_wire_flag = false;
bool wave_norm_flag = false;
bool wave_tang_flag = false;
int segments = 64;
float k = (2 * M_PI) / WL;

Proj2Vec2f *mag[MAG_DEPTH];
Def_proj *def_mag[MAG_DEPTH];/*this second array of projectile pointers 
are used for modeling the flight of the defensive pellets*/
//TODO: introduce projectile/wave hit detection, I.E. if a projectiles y co-ord
//is less than the y value of the sine function at it's x co-ord, recycle it.
Global g =
{
	.frameRateI = 1
};

/*prototype for a helper function, it's at the very end of the file*/
void boatCDhelper(Boat * boot);
void drawOSD();
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
	.gun_elev = 150,
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
	if(tasmania.def_cd > 0)
	{
		tasmania.def_cd -= g.dt;
		if(tasmania.def_cd < 0)
		{
			tasmania.def_cd = 0;
		}
	}
	if(tasmania.cd == 0 && tasmania.shellp == NULL)
	{
		/* find a free projectile */
		/* this is somewhat precarious, if no free projectile
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
	}
	/*identical behaviour to the above statement, except for defensives*/
	if(tasmania.def_cd == 0 && tasmania.dp == NULL)
	{
		for(int i = 0; i < MAG_DEPTH; i++)
		{
			if(def_mag[i]->proj.loaded == false 
			&& def_mag[i]->proj.fired == false)
			{
				def_mag[i]->proj.loaded = true;
				tasmania.dp = def_mag[i];
				tasmania.dp->r = .01;
				printf("new shell loaded into island cannon\n");
				break;
			}
		}
	}
	/*reload boat shells*/
	boatCDhelper(&rightBoat);
	boatCDhelper(&leftBoat);
	/* these update methods are contained in their respective 
	 * files, they essentially allow the shell to be moved with the
	 * movement of the boat's gun as it bobs on the waves*/
	updateBoatShell(&rightBoat);
	updateBoatShell(&leftBoat);
	updateIslandShell(&tasmania);
	
	for(int i = 0; i < MAG_DEPTH; i++)
	{
		if(mag[i]->fired == true)
		{
			/*also need to do hit detection in here*/
			updateProj(mag[i]);
			if(detectIslandHit(mag[i]))
			{
				tasmania.hp--;
				mag[i]->fired = false;
				mag[i]->loaded = false;
			}
			if(detectBoatHit(&rightBoat, mag[i]))
			{
				rightBoat.hp--;
				mag[i]->fired = false;
				mag[i]->loaded = false;
			}
			if(detectBoatHit(&leftBoat, mag[i]))
			{
				leftBoat.hp--;
				mag[i]->fired = false;
				mag[i]->loaded = false;
			}
			/*O(N^2), absolute garbage fire, this isn't good*/
			for(int k = 0; k < MAG_DEPTH; k++)
			{
				if(def_mag[k]->proj.fired == true)
				{
					if(detectIntercept(def_mag[k], mag[i]))
					{
						/*the shell has been intercepted, recycle it*/
						mag[i]->fired = false;
						mag[i]->loaded = false;
					}
				}
			}
		}
		if(def_mag[i]->proj.fired == true)
		{
			updateDefProj(def_mag[i]);
		}
	}

	/*Finally; update framerate tracking information*/
	float fdt = g.t - g.lastFrameRateT;
  	if (fdt > g.frameRateI) 
	{
    	g.frameRate = g.frames / fdt;
    	g.lastFrameRateT = g.t;
    	g.frames = 0;
  	}
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
	drawOSD();
	/*draw trajectories, draw projectiles, draw defense loop through mags*/
	for(int i = 0; i < MAG_DEPTH; i++)
	{
		if((*mag[i]).loaded == true ||(*mag[i]).fired == true)
		{
			drawTraj((mag[i]));
		}
		if((*mag[i]).fired == true)
		{
			drawProj((mag[i]));
		}
		if(def_mag[i]->proj.fired == true)
		{
			drawDefProj(def_mag[i]);
		}
	}

    if((err = glGetError()))
    {
        printf("%s: %x\n", (gluErrorString(err)), err);
    }
   	g.frames++;
    glutSwapBuffers();
}

void init()
{
    /* In this program these OpenGL calls only need to be done once,
      but normally they would go elsewhere, e.g. display */

	/*malloc projectile array in here, these actually never get cleaned up
	 * properly because GLUT doesn't allow one to exit the main loop afaik*/
	int i;
	for(i = 0; i < MAG_DEPTH; i++)
	{
		mag[i] = (Proj2Vec2f*) malloc(sizeof(Proj2Vec2f));
		printf("Projectile memory allocated\n");
		(*mag[i]).fired = false;
		(*mag[i]).loaded = false;
		def_mag[i] = (Def_proj*) malloc(sizeof(Def_proj));
		printf("Defensive projectile memory allocated\n");
		def_mag[i]->proj.fired = false;
		def_mag[i]->proj.loaded = false;
		def_mag[i]->r = .01;
	}
	/*Need to write over the already in memory*/
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(500, 500);
    glutCreateWindow("Island defence 2D");

    init();
    
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void boatCDhelper(Boat * boot)
{
	if(boot->cd > 0)
	{
		/*this is not a robust way of doing this*/
		boot->cd -= g.dt;
		if(boot->cd < 0)
		{
			boot->cd = 0;
		}
	}
	if(boot->def_cd > 0)
	{
		boot->def_cd -= g.dt;
		if(boot->cd < 0)
		{
			boot->cd = 0;
		}
	}
	if(boot->cd == 0 && boot->shellp == NULL)
	{
		for(int i = 0; i < MAG_DEPTH; i++)
		{
			if((*mag[i]).loaded == false && (*mag[i]).fired == false)
			{
				mag[i]->loaded = true;
				boot->shellp = mag[i];
				printf("new shell loaded into leftboat cannon\n");
				break;
			}
		}
	}
	if(boot->dp == NULL && boot->def_cd <= 0)
	{
		for(int i = 0; i < MAG_DEPTH; i++)
		{
			if(def_mag[i]->proj.loaded == false 
			&& def_mag[i]->proj.fired == false)
			{
				def_mag[i]->proj.loaded = true;
				boot->dp = def_mag[i];
				boot->dp->r = .01;
				printf("new defense shell loaded into leftboat cannon\n");
				break;
			}
		}
	}
}

/*I'll be honest, this code is pretty much the same as the 
 * OSD from tute 3 with the exception of the health bars*/
void drawOSD()
{
	char char_buf[30];
  	char * bufp;
  	int w, h;
    
  	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  	glDisable(GL_DEPTH_TEST);
  	glDisable(GL_LIGHTING);

  	glMatrixMode(GL_PROJECTION);
  	glPushMatrix();
  	glLoadIdentity();

  	/* Set up orthographic coordinate system to match the 
     window, i.e. (0,0)-(w,h) */
  	w = glutGet(GLUT_WINDOW_WIDTH);
  	h = glutGet(GLUT_WINDOW_HEIGHT);
  	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

  	glMatrixMode(GL_MODELVIEW);
  	glPushMatrix();
  	glLoadIdentity();

  	/* Frame rate */
  	glColor3f(1.0, 1.0, 0.0);
  	glRasterPos2i(350, 485);
  	snprintf(char_buf, sizeof char_buf, "fr (f/s): %6.0f", g.frameRate);
  	for (bufp = char_buf; *bufp; bufp++)
	{
    	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
	}

  	/* Time per frame */
  	glColor3f(1.0, 1.0, 0.0);
  	glRasterPos2i(350, 470);
  	snprintf(char_buf, sizeof char_buf, "ft (ms/f): %5.0f", 
												1.0 / g.frameRate * 1000.0);
  	for (bufp = char_buf; *bufp; bufp++)
	{
    	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
	}

  	/* Pop modelview */
  	glPopMatrix();  
  	glMatrixMode(GL_PROJECTION);

  	/* Pop projection */
  	glPopMatrix();  
  	glMatrixMode(GL_MODELVIEW);

  	/* Pop attributes */
  	glPopAttrib();
	/*hard coded health bar drawing, drawn as quad strips*/
	glPushMatrix();
	glTranslatef(-.9, .9, 0);
	glColor3f(1,0,0);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,.03,0);
	glVertex3f(leftBoat.hp*.03, .03, 0);
	glVertex3f(leftBoat.hp*.03, 0, 0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,-.03,0);
	glVertex3f(rightBoat.hp*.03, -.03, 0);
	glVertex3f(rightBoat.hp*.03, 0, 0);
	glColor3f(1,1,0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0,-.03,0);
	glVertex3f(0,-.06,0);
	glVertex3f(tasmania.hp*.003, -.06, 0);
	glVertex3f(tasmania.hp*.003, -.03, 0);
	glEnd();

	glPopMatrix();

	return;
}
