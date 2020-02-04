#include "projectiles.h"

#define DT .01 /*Time delta used for trajectory prediction. in seconds*/
#define EXPANSION_RATE .2 /*Rate of expansion per second of defense radius*/
#define MAX_R .15 /*used to limit the lifespan of the defense projectile*/

void drawTraj(const Proj2Vec3f * shell)
{
    /*because this code shouldn't modify any actual attributes 
     * of the shell argument, we copy all the relevant data into local
     * variables and work from them. Expensive space wise, but safe*/
    float x = shell->p.x;
    float y = shell->p.y;
    float z = shell->p.z;

    float dx = shell->d.x;
    float dy = shell->d.y;
    float dz = shell->d.z;

    float wave_y = 0;
    float dyX = 0;
    float dyZ = 0;
    sumWaves(Waves, WAVES, x, z, g.wt, &wave_y, false, &dyX, &dyZ);


    /*euler numerical integration*/

    glDisable(GL_LIGHTING);/*these calls shouldn't be here*/
    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);

    /*wave_y used to determine when the shell falls into the ocean*/
    while(y > wave_y && (x > -X_MAX && x < X_MAX) && (z > -Z_MAX && z < Z_MAX))
    {

        glVertex3f(x, y, z);
        x += dx * DT;
        y += dy * DT;
        z += dz * DT;
        dy += GRAV * DT;
        sumWaves(Waves, WAVES, x, z, g.wt, &wave_y, false, &dyX, &dyZ);
    }
    glEnd();
    /* these are debug statements
    if(wave_y > y)
    {
        printf("wave_y greater than y\n");
        printf("wave_y %f\n", wave_y);
        printf("x %f, y %f, z %f\n", x, y, z);
    }*/
    glEnable(GL_LIGHTING);
}

/*This only gets used while the projectile is in flight*/
void updateProj(Proj2Vec3f * shell)
{
    /*perform numerical integration on projectiles*/

    float dyX = 0; /*these never get used, but i need them to avoid segfaults*/
    float dyZ = 0;
    shell->p.x += shell->d.x * g.dt;
    shell->p.y += shell->d.y * g.dt;
    shell->p.z += shell->d.z * g.dt;
    shell->d.y += GRAV * g.dt;
    float y = 0;
    sumWaves(Waves, WAVES, shell->p.x, shell->p.z, g.wt, &y, false, &dyX, &dyZ);
    /*we're never going to see it again if it goes past these
     * bounds so reset all the variables and make it eligible for use again*/
    if((shell->p.x < -X_MAX || shell->p.x > X_MAX) || 
       (shell->p.z < -Z_MAX || shell->p.z > Z_MAX) || shell->p.y < y)
    {
        
        shell->p.x = 2;
        shell->p.y = 2;
        shell->p.z = 2;
        shell->d.x = 0;
        shell->d.y = 0;
        shell->d.z = 0;
        shell->loaded = false;
        shell->fired = false;
    }
}

/*general purpose projectile drawing, it just draws a circle based on
 * information contained in the projectile struct*/
void drawProj(const Proj2Vec3f * shell)
{
    /*This draws a circle, it should've been named as such*/
    GLfloat c[] = {.7,.7,.7,1};
    GLfloat a[] = {0.0, 0.0, 0.0, 0.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, c); 
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, a); 
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(shell->p.x, shell->p.y, shell->p.z);
    /*this pointless call is used to reset the color*/
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(shell->r, 10, 10);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    /*
    for(float i = 0; i < 2*M_PI; i += .01)
    {
        glVertex3f(shell->p.x + (shell->r * cosf(i)), 
                   shell->p.y + (shell->r * sinf(i)),
                   shell->p.z + (shell->r);
    }*/
    //glEnd();
}

void updateDefProj(Proj2Vec3f * dshell)
{
    dshell->r += g.dt * EXPANSION_RATE;
    if(dshell->r > MAX_R)
    {
        dshell->p.x = 2; /* triggers projectile recycling in updateProj, see
                         * that function for details*/
    }
    updateProj(dshell);
}

bool detectIntercept(const Proj2Vec3f * dshell, const Proj2Vec3f * shell)
{
    /*I think this works, it's a short hand for a chain of methods*/
    float cx = (dshell->p.x) - (shell->p.x);
    float cy = (dshell->p.y) - (shell->p.y);
    float cz = (dshell->p.z) - (shell->p.z);
    float c = sqrt((cx * cx) + (cz * cz) + (cy *cy));
    if(c < dshell->r + shell->r)
    {
        printf("intercept detected\n");
        return true;
    }
    return false;
}
