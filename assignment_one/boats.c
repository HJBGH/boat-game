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
		glTranslatef(Y_GUN_OFFSET,Y_GUN_OFFSET,0);
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
	    float y = AMP * sinf((k * (*boot).x) + ((M_PI/4.0) * g.t));
		float dy = (k * AMP) * cosf((k * (*boot).x) + ((M_PI/4.0) * g.t));
		float rad_theta = atan(dy);
		printf("theta %f\n", rad_theta);
		/*calculate x offset, then y offset*/
		float c = sqrt((X_GUN_OFFSET * X_GUN_OFFSET) + 
						(Y_GUN_OFFSET * Y_GUN_OFFSET)) * BOAT_SCALE;
		float rad_w = atan(Y_GUN_OFFSET/X_GUN_OFFSET);
		float xso = c * (cosf((M_PI/2) - (rad_theta + rad_w)) * (180/M_PI));
		float yso = c * (sinf((M_PI/2) - (rad_theta + rad_w)) * (180/M_PI));
		printf("cosine: %f\n", cosf((M_PI/2) - (rad_theta + rad_w)) * (180/M_PI));
		printf("c: %f\n", c);
		printf("xso: %f, yso: %f\n", xso, yso);
		
		if(!(boot->left))
		{
			xso = -xso;
		}
		xso *= BOAT_SCALE;
		yso *= BOAT_SCALE;

		/*calculate shell x*/
		((*boot).shellp)->p.x = (*boot).x + xso; /*( BOAT_GUN_L 
					* (cosf((M_PI * (*boot).gun_elev) / 180) + theta) *
					BOAT_SCALE);*/
		((*boot).shellp)->p.y = y + yso;
		/*((BOAT_GUN_L * 
						(sinf((M_PI * (*boot).gun_elev) / 180) + theta)) * BOAT_SCALE);*/
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		((*boot).shellp)->d.x = SHELL_S * 
								(cosf((M_PI * (*boot).gun_elev)/180));
		((*boot).shellp)->d.y = SHELL_S * 
								(sinf((M_PI * (*boot).gun_elev)/180));	
	}
}
