#include "projectiles.h"

#define SEGMENTS 16

void drawTraj(const Proj2Vec2f * shell)
{
	/*because this code shouldn't modify any actual attributes 
	 * of the shell argument, we copy all the relevant data into local
	 * variables and work from them. Expensive space wise, but safe*/
	float y = (*shell).p.y;
	float x = (*shell).p.x;
	float dy = (*shell).d.y;
	float dx = (*shell).d.x;

	/*euler numerical integration*/
	glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);
	for(int i = 0; i < SEGMENTS; i++)
	{
		glVertex3f(x, y, 0);
		y += dy;
		x += dx;
	}
	glEnd();
}

void drawProj(const Proj2Vec2f * shell)
{
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2f((*shell).p.x, (*shell).p.y);
	glEnd();
}
