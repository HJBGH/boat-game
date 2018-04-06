#include "includes.h"

#ifndef BANGBANG
#define BANGBANG
#define GRAV -.098 /*not an accurate representation of gravity, but it works
better with this quatity for balance*/
/*taken from projectile tute, best way to do it afaik*/
typedef struct {float x, y;} Vec2f;
typedef struct
{
	bool fired; /*used when deciding when to draw the projectile*/
	bool loaded; /*so two cannons don't use same projectile*/
	float s; /*launch velocity, used to calculate components of and d 
	as the barrel angle changes*/
	float r; /*radius of the projectile, this is also used to model the radius
	of the pellet cloud of the missile defense shells*/
	Vec2f p;
	Vec2f d;
	/* p tracks current position, 
	 * d tracks x and y components of current velocity,
	 * acceleration is handled in update functions */
} Proj2Vec2f; /*a projectile made of 2x vec2f structs*/


void drawTraj(const Proj2Vec2f * shell);/*draw trajectory of projectile*/
void drawProj(const Proj2Vec2f * shell);/*draw a projectile*/
void updateProj(Proj2Vec2f * shell);/*update a projectile*/
void updateDefProj(Proj2Vec2f * dshell);/*wrapper for updateProj, also updates the
radius of the pellet cloud*/
/*detectIntercept is used to detect collisions between defensive projectiles
 * and shells*/
bool detectIntercept(const Proj2Vec2f * dshell, const Proj2Vec2f * shell);

//this declaration is somewhat useless, this only gets used in assignment_one.c
extern Proj2Vec2f *mag[MAG_DEPTH];/*MAG_DEPTH is defined in includes.h*/

#endif

