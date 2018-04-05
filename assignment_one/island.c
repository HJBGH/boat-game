#include "includes.h"
#include "island.h"

void drawIsland(const Island * i)
{
   	glBegin(GL_QUAD_STRIP);
	glColor3f(1,1,0);
	glVertex3f(-ISLAND_X,OCEAN_FLOOR,0);
	glVertex3f(-ISLAND_X,HEIGHT_OVER_X,0);
	glVertex3f(ISLAND_X,OCEAN_FLOOR,0);
	glVertex3f(ISLAND_X,HEIGHT_OVER_X,0);
	glEnd();
	//Do transforms then draw cannon
	glPushMatrix();
	//glLoadIdentity();	
	glTranslatef(0,HEIGHT_OVER_X,0);
	glRotatef(i->gun_elev, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(ISLAND_GUN_L,.025,0);
	glVertex3f(ISLAND_GUN_L,-0.025,0);
	glVertex3f(0,.025,0);
	glVertex3f(0,-0.025,0);
	glEnd();
	glPopMatrix();
	glLoadIdentity();

}

void updateIsland(Island * i)
{
    if(i->gun_rot_s < 0 && i->gun_elev > MIN_ELEV)
        i->gun_elev += i->gun_rot_s * g.dt;
    else if(i -> gun_rot_s > 0 && i->gun_elev < MAX_I_ELEV)
        i->gun_elev += i->gun_rot_s * g.dt;
}

/* I can rework this using the unit circle*/
void updateIslandShell(const Island * i)
{
	if(i->shellp != NULL)
	{
		(i->shellp)->p.x = ISLAND_GUN_L * cosf((M_PI * i->gun_elev) / 180);
								/*I HATE RADIANS*/
			/*muzzle x co-ord*/	
		(i->shellp)->p.y = ISLAND_GUN_L * sinf((M_PI * i->gun_elev) / 180) 
									+ HEIGHT_OVER_X; 
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		(i->shellp)->d.x = SHELL_S * cosf((M_PI * i->gun_elev)/180);
		(i->shellp)->d.y = SHELL_S * sinf((M_PI * i->gun_elev)/180);	
	}
	if(i->dp != NULL)
	{
		(i->dp)->proj.p.x = ISLAND_GUN_L * cosf((M_PI * i->gun_elev) / 180);
								/*I HATE RADIANS*/
			/*muzzle x co-ord*/	
		(i->dp)->proj.p.y = ISLAND_GUN_L * sinf((M_PI * i->gun_elev) / 180) 
									+ HEIGHT_OVER_X; 
		/*muzzle y co-ord*/
		/*calculate initial velocities*/
		(i->dp)->proj.d.x = SHELL_S * cosf((M_PI * i->gun_elev)/180);
		(i->dp)->proj.d.y = SHELL_S * sinf((M_PI * i->gun_elev)/180);	
	}
}

/*The actions to take if the island is hit are managed in assignment_one.c*/
bool detectIslandHit(const Proj2Vec2f * shell)
{
	if((shell->p.x > -ISLAND_X && shell->p.x < ISLAND_X) &&
		(shell->p.y > OCEAN_FLOOR && shell-> p.y < HEIGHT_OVER_X))
		{
			printf("Island hit!\n");
			return true;
		}
	return false;
}
