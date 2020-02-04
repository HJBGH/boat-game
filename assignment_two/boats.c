#include "includes.h"
#include "boats.h"
#include "turrets.h" //need access to turret dimenstions for projectile updates
#define X_GUN_OFFSET -.5
#define Y_GUN_OFFSET .25
#define BOAT_GUN_SCALE .4
#define HIT_RADIUS .09 /*the projectile hits its own boat on launch if this is
any bigger*/
/*I should consider declaring structs to store the vertices of the boats
 * */
void drawBoat(const Boat * boot, float s)
{
    /*use information contained in the boat struct to calculate screen
     * position and rotation*/
    float dyX = 0;
    float dyZ = 0;
    float y = 0;
    //array length
    sumWaves(Waves, WAVES, boot->x, boot->z, g.wt, &y, true, &dyX, &dyZ);
    glPushMatrix();
    //glLoadIdentity();
    glTranslatef(boot->x, y, boot->z);
    glScalef(s, s, s);
    /*functions returns radians*/
    glRotatef((atan(dyX) * (180 / M_PI)), 0, 0, 1);
    glRotatef(-(atan(dyZ) * (180 / M_PI)), 1, 0, 0);
    glRotatef(boot->heading, 0, 1, 0);
    drawAxes(1);
    GLfloat c[] = {boot->colors[0], boot->colors[1]*.75, boot->colors[2]*.75, 1};
    GLfloat a[] = {0.0, 0.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, a); 
    glEnable(GL_LIGHTING);
    glBegin(GL_TRIANGLE_FAN);
    /*drawing hull*/
    /*drawing prow*/
    hbDrawNormalAndVert3f(-1.0, 0.25, 0); 
    hbDrawNormalAndVert3f(-0.5,0.25,0.325);
    hbDrawNormalAndVert3f(-0.5,-0.25,0);
    hbDrawNormalAndVert3f(-0.5,0.25,-0.325);
    hbDrawNormalAndVert3f(-0.5,0.25,0.325);

    glEnd();
    glBegin(GL_QUAD_STRIP);
    /*draw midship*/
    /*these functions are wrong*/
    hbDrawNormalAndVert3f(-0.5,0.25, 0.325);
    hbDrawNormalAndVert3f(0.5,0.25, 0.325);
    hbDrawNormalAndVert3f(-0.5,0.25, -0.325);
    hbDrawNormalAndVert3f(0.5,0.25, -0.325);
    hbDrawNormalAndVert3f(-0.5,-0.25,0);
    hbDrawNormalAndVert3f(0.5,-0.25,0);
    hbDrawNormalAndVert3f(-0.5,0.25, 0.325);
    hbDrawNormalAndVert3f(0.5,0.25, 0.325);
    glEnd();
    //second triangle fan for stern
    /*move things around*/
    glBegin(GL_TRIANGLE_FAN);
    hbDrawNormalAndVert3f(0.75,0.25,0);
    hbDrawNormalAndVert3f(0.5,0.25,0.325);
    hbDrawNormalAndVert3f(0.5,-0.25,0);
    hbDrawNormalAndVert3f(0.5,0.25,-0.325);
    hbDrawNormalAndVert3f(0.5,0.25,0.325);
    glEnd();

    glPushMatrix();
    glTranslatef(0,.3,0);
    /*draw bridge*/
    glutSolidCube(.3);
    glPopMatrix();

    glPushMatrix();
    /*draw gun, this requires some reasoning about boat orientation*/
    glTranslatef(X_GUN_OFFSET, 0, 0);
    glTranslatef(0, Y_GUN_OFFSET, 0);
    glScalef(BOAT_GUN_SCALE, BOAT_GUN_SCALE, BOAT_GUN_SCALE);
    drawTurret(&(boot->bt), c);
    glPopMatrix();
    glPopMatrix();
}   

/*This is very tightly bound to the left/right member variable, it isn't
 * really a good solution*/
void updateBoat(Boat * boot)
{
    //TODO: fix this function to use basic scripted AI and account for the
    //now 3d world
    if(boot->s < 0)
    {
        if(boot->x > -X_MAX)
            boot->x += boot->s * g.dt;
        else if(boot->x > CENTER+.15)//.15 begin island width
            boot->x += boot->s * g.dt;
    }
    /* gun updates need to be changed to account for the implementation of 
     * turrents*/
}

/*The defense projectile also gets updated in here*/
void updateBoatShell(const Boat * boot)
{   
    /*
    float y = 0;
    float dyZ = 0; 
    float dyX = 0;
    sumWaves(Waves, WAVES, boot->x, boot->z, g.wt, &y, true, &dyX, &dyZ);

    //note to self: we can't use the same method we used in assignment 1 for 
    //this
    float ts = BOAT_GUN_SCALE * BOAT_SCALE;
    float hOff = sqrt((X_GUN_OFFSET*X_GUN_OFFSET) 
                    + (Y_GUN_OFFSET*Y_GUN_OFFSET)); //Hull offset
    float gunx = (cosf((boot->heading * M_PI) / 180) * Xgo) +
          (cosf((boot->bt.facing * M_PI) / 180) * (T_GUN_L * ts));
    float gunz = ((sinf((boot->heading * M_PI) / 180) * Xgo) + 
          (sinf((boot->bt.facing * M_PI) / 180) * (T_GUN_L * ts)));
                //calculating guny needs to factor in turret dimensions
    float guny = Y_GUN_OFFSET +
         (sinf((boot->bt.gun_elev * M_PI) / 180) * (T_GUN_L * ts));
    float wz = atan2(guny,gunx) + atan(dyX);
    float wx = atan2(guny,gunz) + atan(dyZ);*/
    /*The useful result of all the calculations above is that wx has the angle
     * of the rotation in world-space of the gun muzzle around the z axis, we
     * can use the unit circle to find the x and y co-ordinate of it in x 
     * space*/

    /*
    if(boot->bt.shellp != NULL)
    {
        (boot->bt.shellp)->p.x = boot->x + (cosf(wz) * BOAT_SCALE);
        (boot->bt.shellp)->p.z = boot->z + (sinf(wx) * BOAT_SCALE);
        (boot->bt.shellp)->p.y = y + (sinf(wz) * BOAT_SCALE);
        wz =  ((boot->bt.facing * M_PI) / 180) + atan(dyX);
        wx =  ((boot->bt.gun_elev * M_PI) / 180) + atan(dyZ);

        (boot->bt.shellp)->d.x = (cosf(wz) * SHELL_S);
        (boot->bt.shellp)->d.z = (sinf(wx) * SHELL_S);
        (boot->bt.shellp)->d.y = (sinf(wz) * SHELL_S);
    }
    if(boot->bt.dp != NULL)
    {
        (boot->bt.dp)->p.x = boot->x + (cosf(wz) * BOAT_SCALE);
        (boot->bt.dp)->p.z = boot->z + (sinf(wx) * BOAT_SCALE);
        (boot->bt.dp)->p.y = y + (sinf(wz) * BOAT_SCALE);//this is probably incorrect
        wz =  ((boot->bt.facing * M_PI) / 180) + atan(dyX);
        wx =  ((boot->bt.gun_elev * M_PI) / 180) + atan(dyZ);

        (boot->bt.dp)->d.x = (cosf(wz) * SHELL_S);
        (boot->bt.dp)->d.z = (sinf(wx) * SHELL_S);
        (boot->bt.dp)->d.y = (sinf(wz) * SHELL_S);
    }   */
}

//this hit detection still only works in 2d.
bool detectBoatHit(const Boat * boot, const Proj2Vec3f * shell)
{
    /*I can take some measures to avoid calculation in here based on the position
     * of the shell. for example, if the shell has a negative x co-ord and 
     * the boat a positive one, we can return false without doing any further
     * computing, effectively the if statements below form a quad tree of depth
     * 1*/      
    float y = 0;
    float dyX = 0;
    float dyZ = 0;
    sumWaves(Waves, WAVES, boot->x, boot->z, g.wt, &y, true, &dyX, &dyZ);
    if((boot->x < 0 && shell->p.x > 0) || (boot->x > 0 && shell->p.x < 0))
    {
        return false;
    }
    if((y< 0 && shell->p.y > 0) || (y > 0 && shell->p.y < 0))
    {
        return false;
    }
    /*calculate the hypotenuse of the triangle formed by the shell, the centre
      of the boat ( it's x,y co-ords) and a third point that is the intersection
      of the shell x axis and the boat y axis.*/
    float cx = (boot->x - (shell->p.x));
    float cy = (fabs(y) - (shell->p.y));
    float cz = (boot->z - (shell->p.z));
    float c = sqrt((cx * cx) + (cy *cy) + (cz * cz));
    if(c < HIT_RADIUS + shell->r)
    {
        printf("boat hit detected\n");
        return true;
    }
    return false;
}

