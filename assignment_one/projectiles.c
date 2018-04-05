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

void drawProj(const Proj2Vec2f * shell)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1,1,1);
	glVertex2f((*shell).p.x, (*shell).p.y);
	glEnd();
}

/*only function allowed to change shell, this is irrelevant considering
 * how much I change them in assignment_one.c*/
/*This only gets used while the projectile is in flight*/
void updateProj(Proj2Vec2f * shell)
{
	//printf("Updating the projectile at address %p\n", shell);
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

void drawDefProj(const Def_proj * dp)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0,1.0,1.0);
	for(float i = 0; i < 2*M_PI; i += .01)
	{
		glVertex3f(dp->proj.p.x + (dp->r * cosf(i)), 
				   dp->proj.p.y + (dp->r * sinf(i)), 0);
	}
	glEnd();
}

void updateDefProj(Def_proj * dp)
{
	dp->r += g.dt * EXPANSION_RATE;
	if(dp->r > MAX_R)
	{
		dp->proj.p.x = 2; /* triggers projectile recycling in updateProj, see
						 * that function for details*/
	}
	updateProj(&(dp->proj));
}

bool detectIntercept(const Def_proj * dp, const Proj2Vec2f * shell)
{
	if((dp->proj.p.x < 0 && shell->p.x > 0) || 
	(dp->proj.p.x > 0 && shell->p.x < 0))
	{
		return false;
	}
	if((dp->proj.p.y < 0 && shell->p.y > 0) || 
	(dp->proj.p.y > 0 && shell->p.y < 0))
	{
		return false;
	}
	/*calculate the hypotenuse of the triangle formed by the shell, the centre
	  of the boat ( it's x,y co-ords) and a third point that is the intersection
	  of the shell x axis and the boat y axis.*/
	float cx = (fabs(dp->proj.p.x) - fabs(shell->p.x));
	float cy = (fabs(dp->proj.p.y) - fabs(shell->p.y));
	float c = sqrt((cx * cx) + (cy *cy));
	//printf("c: %f\n", c);
	if(c < dp->r)
	{
		printf("intercept detected\n");
		return true;
	}
	return false;

}
