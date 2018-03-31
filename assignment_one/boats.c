#include "includes.h"
#include "boats.h"
#define WHY_IS_X_ZERO 90


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
		glTranslatef(.5,.25,0);
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
		glTranslatef(-.5,.25,0);
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

void updateBoatShell(const Boat * boot)
{
	if((*boot).shellp != NULL)
	{
	    float y = AMP * sinf((k * (*boot).x) + ((M_PI/4.0) * g.t));
		((*boot).shellp)->p.x = (*boot).x;/*BOAT_GUN_L * 
							cosf((M_PI * (*boot).gun_elev) / 180);*/
				/*muzzle x co-ord*/	
		printf("Right boat gun elev -> %f\n", ((*boot).gun_elev));
		printf("I don't even know %f\n", ((*boot).shellp)->p.x);
		((*boot).shellp)->p.y = y + ((BOAT_GUN_L * 
						sinf((M_PI * (*boot).gun_elev) / 180)) * BOAT_SCALE);
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		((*boot).shellp)->d.x = SHELL_S * 
								cosf((M_PI * (*boot).gun_elev)/180);
		((*boot).shellp)->d.y = SHELL_S * 
								sinf((M_PI * (*boot).gun_elev)/180);	
	}
}
