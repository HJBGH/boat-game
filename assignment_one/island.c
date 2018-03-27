#include "includes.h"
#include "island.h"
#define HEIGHT_OVER_X .2

void drawIsland(const Island * i)
{
   	glBegin(GL_QUAD_STRIP);
	glColor3f(1,1,0);
	glVertex3f(-.15,-1,0);
	glVertex3f(-.15,HEIGHT_OVER_X,0);
	glVertex3f(.15,-1,0);
	glVertex3f(.15,HEIGHT_OVER_X,0);
	glEnd();
	//Do transforms then draw cannon
	glPushMatrix();
	glTranslatef(0,HEIGHT_OVER_X,0);
	/*TODO !!!!!!!!! double check cannon specs*/
	glRotatef((*i).gun_elev, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-.025,0,0);
	glVertex3f(-.025,.15,0);
	glVertex3f(.025,0,0);
	glVertex3f(.025,.15,0);
	glEnd();
	glPopMatrix();

}
