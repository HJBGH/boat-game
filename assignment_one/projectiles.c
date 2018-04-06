#include "projectiles.h"

#define DT .01 /*Time delta used for trajectory prediction. in seconds*/
#define EXPANSION_RATE .2 /*Rate of expansion per second of defense radius*/
#define MAX_R .15 /*used to limit the lifespan of the defense projectile*/

void drawTraj(const Proj2Vec2f * shell)
{
    /*because this code shouldn't modify any actual attributes 
     * of the shell argument, we copy all the relevant data into local
     * variables and work from them. Expensive space wise, but safe*/
    float y = shell->p.y;
    float x = shell->p.x;
    float dy = shell->d.y;
    float dx = shell->d.x;
    float wave_y = AMP * sinf((k * x) + ((M_PI/4.0) * g.wt));

    /*euler numerical integration*/
    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);

    /*wave_y used to determine when the shell falls into the ocean*/
    while(y > wave_y && (x > L_MAX && x < R_MAX))
    {
        glVertex3f(x, y, 0);
        y += dy * DT;
        x += dx * DT;
        dy += GRAV * DT;
        wave_y = AMP * sinf((k * x) + ((M_PI/4.0) * g.wt));
    }
    glEnd();
}

/*This only gets used while the projectile is in flight*/
void updateProj(Proj2Vec2f * shell)
{
    /*perform numerical integration on projectiles*/
    shell->p.y += (*shell).d.y * g.dt;
    shell->p.x += (*shell).d.x * g.dt;
    shell->d.y += GRAV * g.dt;
    float y = AMP * sinf((k * shell->p.x) + ((M_PI/4.0) * g.wt));

    /*we're never going to see it again if it goes past these
     * bounds so reset all the variables and make it eligible for use again*/
    if(((*shell).p.x < L_MAX || (*shell).p.x > R_MAX) || (*shell).p.y < y)
    {
        (*shell).p.y = 2;
        (*shell).p.x = 2;
        (*shell).d.y = 0;
        (*shell).d.x = 0;
        (*shell).loaded = false;
        (*shell).fired = false;
    }
}

/*general purpose projectile drawing, it just draws a circle based on
 * information contained in the projectile struct*/
void drawProj(const Proj2Vec2f * shell)
{
    /*This draws a circle, it should've been named as such*/
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0,1.0,1.0);
    for(float i = 0; i < 2*M_PI; i += .01)
    {
        glVertex3f(shell->p.x + (shell->r * cosf(i)), 
                   shell->p.y + (shell->r * sinf(i)), 0);
    }
    glEnd();
}

void updateDefProj(Proj2Vec2f * dshell)
{
    dshell->r += g.dt * EXPANSION_RATE;
    if(dshell->r > MAX_R)
    {
        dshell->p.x = 2; /* triggers projectile recycling in updateProj, see
                         * that function for details*/
    }
    updateProj(dshell);
}

bool detectIntercept(const Proj2Vec2f * dshell, const Proj2Vec2f * shell)
{
    /*The two if statements below effectively divide the screen space
     * into a quad tree of depth 1*/
    if((dshell->p.x < 0 && shell->p.x > 0) || 
    (dshell->p.x > 0 && shell->p.x < 0))
    {
        return false;
    }
    if((dshell->p.y < 0 && shell->p.y > 0) || 
    (dshell->p.y > 0 && shell->p.y < 0))
    {
        return false;
    }
    /*calculate the hypotenuse of the triangle formed by the shell, the centre
      of the boat ( it's x,y co-ords) and a third point that is the intersection
      of the shell x axis and the boat y axis.*/
    float cx = (fabs(dshell->p.x) - fabs(shell->p.x));
    float cy = (fabs(dshell->p.y) - fabs(shell->p.y));
    float c = sqrt((cx * cx) + (cy *cy));
    if(c < dshell->r + shell->r)
    {
        printf("intercept detected\n");
        return true;
    }
    return false;

}
