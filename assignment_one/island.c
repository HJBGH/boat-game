#include "includes.h"
#include "island.h"

void drawIsland(const Island * i)
{
   	glBegin(GL_QUAD_STRIP);
	glColor3f(1,1,0);
	glVertex3f(-.15,OCEAN_FLOOR,0);
	glVertex3f(-.15,HEIGHT_OVER_X,0);
	glVertex3f(.15,OCEAN_FLOOR,0);
	glVertex3f(.15,HEIGHT_OVER_X,0);
	glEnd();
	//Do transforms then draw cannon
	glPushMatrix();
	glTranslatef(0,HEIGHT_OVER_X,0);
	/*TODO !!!!!!!!! double check cannon specs*/
	glRotatef((*i).gun_elev, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-.025,0,0);
	glVertex3f(-.025,ISLAND_GUN_L,0);
	glVertex3f(.025,0,0);
	glVertex3f(.025,ISLAND_GUN_L,0);
	glEnd();
	glPopMatrix();

}
