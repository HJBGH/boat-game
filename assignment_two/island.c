#include "includes.h"
#include "island.h"

void drawIsland(const Island * i)
{
    GLfloat c[] = {.9, .9, 0, 1};
    //GLfloat spec[] = {.7, .7, .7, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, c);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, spec);


    glColor3f(1,1,0);
    glPushMatrix();
    glTranslatef(0, -(HEIGHT - HEIGHT_OVER_X), 0);
    hbDrawCylinder(HEIGHT, ISLAND_X, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,HEIGHT_OVER_X,0);
    //printf("gun island elevation: %f\n", i->gun_elev);
    glScalef(TURRET_SCALE, TURRET_SCALE, TURRET_SCALE);
    drawTurret(&(i->it), c);
    glPopMatrix();
}

void updateIsland(Island * i)
{
    updateTurret(&(i->it), MIN_ELEV, MAX_I_ELEV);
}

/* because of the difference in the movement between the turrets
 * on the island the turrets on the boats the updateIslandShell and
 * updateBoatShell functions are still used. */
void updateIslandShell(const Island * i)
{ 
    if(i->it.shellp != NULL)
    {
        /*muzzle y co-ord*/ 
        (i->it.shellp)->p.y = ISLAND_GUN_L * sinf((M_PI * i->it.gun_elev) / 180) 
                                + HEIGHT_OVER_X + (T_WIDTH/2 * TURRET_SCALE); 
        /*intermediate var*/
        float a = ISLAND_GUN_L * cosf((M_PI * i->it.gun_elev) / 180);
        /*muzzle x co-ord*/
        (i->it.shellp)->p.x = a * cosf((M_PI * i->it.facing) / 180);
        //invert the z result to flip it over the x axis
        (i->it.shellp)->p.z = -(a * sinf((M_PI * i->it.facing) / 180));

        /*calculate initial velocities*/
        (i->it.shellp)->d.y = SHELL_S * sinf((M_PI * i->it.gun_elev)/180);    
        a = SHELL_S * cosf((M_PI * i->it.gun_elev)/180);
        (i->it.shellp)->d.x = a * cosf((M_PI * i->it.facing) /180);
        (i->it.shellp)->d.z = -(a * sinf((M_PI * i->it.facing) / 180));
    }
    if(i->it.dp != NULL)
    {
        (i->it.dp)->p.y = ISLAND_GUN_L * sinf((M_PI * i->it.gun_elev) / 180) 
                            + HEIGHT_OVER_X + (T_WIDTH/2 * TURRET_SCALE); 
        float a = ISLAND_GUN_L * cosf((M_PI * i->it.gun_elev) / 180);
        (i->it.dp)->p.x = ISLAND_GUN_L * cosf((M_PI * i->it.facing) / 180);
        (i->it.dp)->p.z = -(ISLAND_GUN_L * sinf((M_PI * i->it.facing) / 180));

        (i->it.dp)->d.y = SHELL_S * sinf((M_PI * i->it.gun_elev)/180);    
        a = SHELL_S * cosf((M_PI * i->it.gun_elev)/180);
        (i->it.dp)->d.x = a * cosf((M_PI * i->it.facing)/180);
        (i->it.dp)->d.z = -(a * sinf((M_PI * i->it.facing)/180));
    }
}

/*The actions to take if the island is hit are managed in assignment_one.c*/
bool detectIslandHit(const Proj2Vec3f * shell)
{
    /*basic static bounding box style collision, I've fudged the hit
     * detection here by using the x,y co-ordinates of the shell instead of 
     * its radius.*/
    //ISLAND_X is effectively just the radius at this point
    if((shell->p.x > -ISLAND_X && shell->p.x < ISLAND_X) &&
        (shell->p.y > OCEAN_FLOOR && shell-> p.y < HEIGHT_OVER_X) &&
        (shell->p.z > -ISLAND_X && shell->p.z < ISLAND_X))
        {
            printf("Island hit!\n");
            return true;
        }
    return false;
}
