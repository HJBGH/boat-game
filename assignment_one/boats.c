#include "includes.h"
#include "boats.h"
#define X_GUN_OFFSET .5
#define Y_GUN_OFFSET .25
#define HIT_RADIUS .15
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
		/*Use the unit circle to set new co-ordinates in the appropriate
		 * vectors*/
		(boot->shellp)->p.x = boot->x + (cosf(w) * BOAT_SCALE);
		(boot->shellp)->p.y = y + (sinf(w) * BOAT_SCALE);
		/*set the direction vector as well*/
		w =  ((boot->gun_elev * M_PI) / 180) + atan(dy);

		(boot->shellp)->d.x = (cosf(w) * SHELL_S);
		(boot->shellp)->d.y = (sinf(w) * SHELL_S);
	}
}

//TODO: test this method to find 
bool detectBoatHit(const Boat * boot, const Proj2Vec2f * shell)
{
	/*I can take some measures to avoid calculation in here based on the position
	 * of the shell. for example, if the shell has a negative x co-ord and 
	 * the boat a positive one, we can return false without doing any further
	 * computing*/	    
	float y = AMP * sinf((k * boot->x) + ((M_PI/4.0) * g.t));
	if((boot->x < 0 && (shell->p).x > 0) || (boot->x > 0 && (shell->p).x < 0))
	{
		return false;
	}
	if((y< 0 && (shell->p).y > 0) || (y > 0 && (shell->p).y < 0))
	{
		return false;
	}
	/*calculate the hypotenuse of the triangle formed by the shell, the centre
	  of the boat ( it's x,y co-ords) and a third point that is the intersection
	  of the shell x axis and the boat y axis.*/
	float cx = (fabs(boot->x) - fabs((shell->p).x));
	float cy = (fabs(y) - fabs((shell->p).y));
	float c = sqrt((cx * cx) + (cy *cy));
	//printf("c: %f\n", c);
	if(c < HIT_RADIUS)
	{
		printf("boat hit detected\n");
		return true;
	}
	return false;
}

