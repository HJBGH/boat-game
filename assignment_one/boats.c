#include "includes.h"
#include "boats.h"
#define X_GUN_OFFSET .5
#define Y_GUN_OFFSET .25
#define HIT_RADIUS .09 /*the projectile hits its own boat on launch if this is
any bigger*/
/*I should consider declaring structs to store the vertices of the boats
 * */
void drawBoat(const Boat * boot, float s)
{
	/*use information contained in the boat struct to calculate screen
	 * position and rotation*/
	float dy = (k * AMP) * cosf((k * (*boot).x) + ((M_PI/4.0) * g.wt));
    float y = AMP * sinf((k * (*boot).x) + ((M_PI/4.0) * g.wt));

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

/*This is very tightly bound to the left/right member variable, it isn't
 * really a good solution*/
void updateBoat(Boat * boot)
{
    if(boot->s < 0)
    {
        if(boot->left && boot->x > L_MAX)
            boot->x += boot->s * g.dt;
        else if(!(boot->left) && boot->x > CENTER+.15)//.15 begin island width
            boot->x += boot->s * g.dt;
    }
    else if(boot->s > 0)
    {
        if(boot->left && boot->x < CENTER-.15)
            boot->x += boot->s * g.dt;
        else if(!(boot->left) && boot->x < R_MAX)
            boot->x += boot->s * g.dt;
    }
    /*update gun elevation also*/
    if(boot->gun_rot_s < 0)
    {
        if(boot->left && boot->gun_elev > MIN_ELEV)
            boot->gun_elev += boot->gun_rot_s * g.dt;
        else if(!(boot->left) && boot->gun_elev > MAX_B_ELEV)
            boot->gun_elev += boot->gun_rot_s * g.dt;
    }
    else if(boot->gun_rot_s > 0)
    {
        if(boot->left && boot->gun_elev < MAX_B_ELEV)
            boot->gun_elev += boot->gun_rot_s * g.dt;
        else if(!(boot->left) && boot->gun_elev < MAX_I_ELEV)
            boot->gun_elev += boot->gun_rot_s * g.dt;
    }
}

/*The defense projectile also gets updated in here*/
void updateBoatShell(const Boat * boot)
{	    
	float y = AMP * sinf((k * boot->x) + ((M_PI/4.0) * g.wt));
	float dy = (k * AMP) * cosf((k * boot->x) + ((M_PI/4.0) * g.wt)); 
		/*I have to find the angle at the centre of the boat, with the sides 
		 * being X_GUN_OFFSET + (cos(gun_elev) * BOAT_GUN_L) and Y_GUN_OFFSET
		 * + (sin(gun_elev) * BOAT_GUN_L)*/
	float go = boot->left ? X_GUN_OFFSET : -X_GUN_OFFSET;
	float gunx = go + (cosf((boot->gun_elev * M_PI) / 180) * BOAT_GUN_L);
	float guny = Y_GUN_OFFSET +
				(sinf((boot->gun_elev * M_PI) / 180) * BOAT_GUN_L);
	float w = atan2(guny,gunx) + atan(dy);
    /*The useful result of all the calculations above is that w has the angle
     * of the rotation in world-space of the gun muzzle around the z axis, we
     * can use the unit circle to find the x and y co-ordinate of it in x 
     * space*/


	if(boot->shellp != NULL)
	{
		/*Calculate useful information*/
		/*Use the unit circle to set new co-ordinates in the appropriate
		 * vectors*/
		(boot->shellp)->p.x = boot->x + (cosf(w) * BOAT_SCALE);
		(boot->shellp)->p.y = y + (sinf(w) * BOAT_SCALE);
		/*set the direction vector as well*/
		w =  ((boot->gun_elev * M_PI) / 180) + atan(dy);

		(boot->shellp)->d.x = (cosf(w) * SHELL_S);
		(boot->shellp)->d.y = (sinf(w) * SHELL_S);
	}
	if(boot->dp != NULL)
	{
        /*do the exact same calculations as before, except use the to 
         * set up the defensive projectile*/
		(boot->dp)->p.x = boot->x + (cosf(w) * BOAT_SCALE);
		(boot->dp)->p.y = y + (sinf(w) * BOAT_SCALE);
		/*set the direction vector as well*/
		w =  ((boot->gun_elev * M_PI) / 180) + atan(dy);

		(boot->dp)->d.x = (cosf(w) * SHELL_S);
		(boot->dp)->d.y = (sinf(w) * SHELL_S);
	}	
}

bool detectBoatHit(const Boat * boot, const Proj2Vec2f * shell)
{
	/*I can take some measures to avoid calculation in here based on the position
	 * of the shell. for example, if the shell has a negative x co-ord and 
	 * the boat a positive one, we can return false without doing any further
	 * computing, effectively the if statements below form a quad tree of depth
     * 1*/	    
	float y = AMP * sinf((k * boot->x) + ((M_PI/4.0) * g.wt));
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
	if(c < HIT_RADIUS + shell->r)
	{
		printf("boat hit detected\n");
		return true;
	}
	return false;
}

