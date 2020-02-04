#include "includes.h"
#include "turrets.h"

void drawTurret(const Turret * turret, GLfloat * c)
{
    //float h; //used for normalization, a normalization function may be useful
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, c);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);

    glEnable(GL_LIGHTING); 
    glPushMatrix();
    //glScalef(TURRET_SCALE, TURRET_SCALE, TURRET_SCALE);
    glRotatef(turret->facing, 0, 1, 0);
    /*draw barrel*/
    glPushMatrix();
    glTranslatef(0, T_WIDTH/2, 0);
    //printf("gun elevation: %f\n", turret->gun_elev);
    /*minus 90 is an appropriate rotation offset considering how hbDrawCylinder
     * works*/
    glRotatef(turret->gun_elev - 90, 0, 0, 1);
    hbDrawCylinder(T_GUN_L, T_GUN_W/2, 6);
    glPopMatrix();
    /*draw base*/
    //glutSolidCube(T_WIDTH);
    glPushMatrix();
    //draw cylinder to act as a turret cap
    glTranslatef(0, T_WIDTH/2, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -T_WIDTH/2, 0);
    hbDrawCylinder(T_WIDTH, T_WIDTH/2, 12);
    drawAxes(5);
    glPopMatrix();
    /*
    glPushMatrix();
    glRotatef(turret->facing, 0, 1, 0);
    hbDrawCylinder(1, .0725, 6);
    glPopMatrix();*/
    glPopMatrix();
    glDisable(GL_LIGHTING);
}

void updateTurret(Turret * t, float min_elev, float max_elev)
{
    if(t->gun_elev_s < 0 && t->gun_elev > min_elev)
        t->gun_elev += t->gun_elev_s * g.dt;
    else if(t->gun_elev_s > 0 && t->gun_elev < max_elev)
        t->gun_elev += t->gun_elev_s * g.dt;
    if(t->rot_s != 0)
    {
        t->facing += t->rot_s * g.dt;
        //ensure values wrap around
        if(t->facing > 360)
        {
            t->facing = 0;
        }
        else if(t->facing < 0)
        {
            t->facing = 360;
        }
    }
    //i->gun_elev += i->gun_rot_s * g.dt;
}
