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
	//glLoadIdentity();	
	glTranslatef(0,HEIGHT_OVER_X,0);
	/*TODO !!!!!!!!! double check cannon specs*/
	glRotatef((*i).gun_elev, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(ISLAND_GUN_L,.025,0);
	glVertex3f(ISLAND_GUN_L,-0.025,0);
	glVertex3f(0,.025,0);
	glVertex3f(0,-0.025,0);
	glEnd();
	glPopMatrix();
	glLoadIdentity();

}

void updateIslandShell(const Island * i)
{
	if(i->shellp != NULL)
	{
		(i->shellp)->p.x = ISLAND_GUN_L * cosf((M_PI * i->gun_elev) / 180);
								/*I HATE RADIANS*/
			/*muzzle x co-ord*/	
		printf("Tasmania gun elev -> %f\n", (i->gun_elev));
		printf("I don't even know %f\n", (i->shellp)->p.x);
		(i->shellp)->p.y = ISLAND_GUN_L * sinf((M_PI * i->gun_elev) / 180) 
									+ HEIGHT_OVER_X; 
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		(i->shellp)->d.x = SHELL_S * cosf((M_PI * i->gun_elev)/180);
		(i->shellp)->d.y = SHELL_S * sinf((M_PI * i->gun_elev)/180);	
	}
}
