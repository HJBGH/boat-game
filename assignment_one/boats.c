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
	if(boot->shellp != NULL)
	{
		/*Calculate useful information*/
	    float y = AMP * sinf((k * boot->x) + ((M_PI/4.0) * g.t));
		float dy = (k * AMP) * cosf((k * boot->x) + ((M_PI/4.0) * g.t)); 

		/*I have to find the angle at the centre of the boat, with the sides 
		 * being X_GUN_OFFSET + (cos(gun_elev) * BOAT_GUN_L) and Y_GUN_OFFSET
		 * + (sin(gun_elev) * BOAT_GUN_L)*/
		float go = boot->left ? X_GUN_OFFSET : -X_GUN_OFFSET;
		float gunx = go + (cosf((boot->gun_elev * M_PI) / 180) * BOAT_GUN_L);
		float guny = Y_GUN_OFFSET +
					(sinf((boot->gun_elev * M_PI) / 180) * BOAT_GUN_L);
		float w = atan2(guny,gunx) + atan(dy);

		if(!(boot->left)) printf("right w: %f\n", w);
		if(!(boot->left)) printf("right gunx: %f\n", gunx);
		if(!(boot->left)) printf("right theta: %f\n", atan(dy));
		if(!(boot->left)) printf("right p: %f\n", atan2(guny,gunx));
		/*Use the unit circle to set new co-ordinates in the appropriate
		 * vectors*/
		(boot->shellp)->p.x = boot->x + (cosf(w) * BOAT_SCALE);
		(boot->shellp)->p.y = y + (sinf(w) * BOAT_SCALE);
		/*set the direction vector as well*/
		(boot->shellp)->d.x = SHELL_S * cosf(w);
		(boot->shellp)->d.y = SHELL_S * sinf(w);
	}
}
