#include "includes.h"
#include "boats.h"
#define X_GUN_OFFSET .5
#define Y_GUN_OFFSET .25

/*TODO: Consider declaring structs to store the vertices of the boats
 * */
void drawBoat(const Boat * boot, float s)
{
	/*use information contained in the boat struct to calculate screen
	 * position and rotation*/
	float dy = (k * AMP) * cosf((k * (*boot).x) + ((M_PI/4.0) * g.t));
    float y = AMP * sinf((k * (*boot).x) + ((M_PI/4.0) * g.t));

	glPushMatrix();
	glLoadIdentity();
	glTranslatef((*boot).x, y, 0);
	glScalef(s, s, s);
	/*functions returns radians*/
	glRotatef((atan(dy) * (180 / M_PI)), 0,0,1);
	drawAxes(1);
	glColor3f((*boot).colors[0], (*boot).colors[1], (*boot).colors[2]);
	glBegin(GL_POLYGON);
	/*drawing hull*/
	glVertex2f(-.5,-.25);
	glVertex2f(.5, -0.25);
	glVertex2f(1.0, 0.25);
	glVertex2f(-1.0, 0.25);
	glEnd();
	/*move things around*/
	glPushMatrix();
	glTranslatef(0,.5,0);
	/*draw bridge*/
	glBegin(GL_POLYGON);
	glVertex2f(-0.25, -0.25);
	glVertex2f(0.25, -0.25);
	glVertex2f(0.25, 0.25);
	glVertex2f(-0.25, 0.25);
	glEnd();

	glPopMatrix();
	glPushMatrix();
	/*draw gun, this requires some reasoning about boat orientation*/
	if((*boot).left)
	{
		glTranslatef(X_GUN_OFFSET,Y_GUN_OFFSET,0);
		glRotatef((*boot).gun_elev, 0, 0, 1);
		//glTranslatef(0,-.0625,0);
		glBegin(GL_POLYGON);
		glVertex2f(0, -.0625);
		glVertex2f(BOAT_GUN_L, -.0625);
		glVertex2f(BOAT_GUN_L, .0625);
		glVertex2f(0, .0625);
	}
	else
	{
		glTranslatef(-X_GUN_OFFSET,Y_GUN_OFFSET,0);
		glRotatef((*boot).gun_elev, 0, 0, 1);
		//glTranslatef(0,-.0625,0);
		glBegin(GL_POLYGON);
		glVertex2f(0, -.0625);
		glVertex2f(BOAT_GUN_L, -.0625);
		glVertex2f(BOAT_GUN_L, .0625);
		glVertex2f(0, .0625);
	}			
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glLoadIdentity(); //just to be safe
}	

/*TODO: refactor so that I'm not using '->' and '(*boot).' interchangebly*/
/*ALSO: add some better comments*/
void updateBoatShell(const Boat * boot)
{
	if((*boot).shellp != NULL)
	{
		/*some reasoning about boat orientation needs to be done.*/
		float gun_elev = boot->gun_elev;
		if(!(boot->left))
		{
			gun_elev = 180-gun_elev;
		}
	    float y = AMP * sinf((k * (*boot).x) + ((M_PI/4.0) * g.t));
		float dy = (k * AMP) * cosf((k * (*boot).x) + ((M_PI/4.0) * g.t));
		float theta = atan(dy);
		/*calculate x offset, then y offset, with respect to the boat*/
		float x1 = BOAT_GUN_L * cosf((gun_elev * M_PI)/180);
		float y1 = BOAT_GUN_L * sinf((gun_elev * M_PI)/180);
		printf("gun_elev: %f\n", gun_elev);
		printf("x1: %f\n", x1);
		float xso = X_GUN_OFFSET + x1;
		float yso = Y_GUN_OFFSET + y1;
		/*calculate final global x/y position*/
		float w = atan(yso/xso) + theta;
		printf("theta %f\n", theta);
		printf("w %f\n", w);
	
		/*calculate shell x*/
		if(!(boot->left))
		{
			(boot->shellp)->p.x =  (*boot).x + (-1 *(cosf(w) * BOAT_SCALE));
		}
		else
		{
			((*boot).shellp)->p.x = (*boot).x + (cosf(w) * BOAT_SCALE);
		}
		((*boot).shellp)->p.y = y + (sinf(w) * BOAT_SCALE);
		
		printf("xso: %f, yso: %f\n", xso, yso);
		printf("x: %f, y: %f\n", ((*boot).shellp)->p.x,((*boot).shellp)->p.y);

		/*calculate initial velocities*/
		if(!(boot->left))
		{
			((*boot).shellp)->d.x = SHELL_S * 
								((cosf((M_PI * (*boot).gun_elev)/180))
								);
			((*boot).shellp)->d.y = SHELL_S * 
								((sinf((M_PI * (*boot).gun_elev)/180))
								);	
		}
	}
}
