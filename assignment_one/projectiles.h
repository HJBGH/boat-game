#include "includes.h"

#ifndef BANGBANG
#define BANGBANG
#define GRAV_CONST -9.8
/*taken from projectile tute, best way to do it afaik*/
typedef struct {float x, y;} Vec2f;
typedef struct
{
	bool fired; /*used when deciding how to draw this*/
	bool loaded; /*so two cannons don't use same projectile*/
	float s; /*launch velocity, used to calculate components of p and d 
	as the barrel angle changes*/
	Vec2f p;
	Vec2f d;
	/* p tracks current position, 
	 * d tracks x and y components of current velocity,
	 * acceleration is handled externally*/
} Proj2Vec2f; /*a projectile made of 2x vec2f structs*/

void drawTraj(const Proj2Vec2f * shell);
void drawProj(const Proj2Vec2f * shell);

extern Proj2Vec2f *mag[MAG_DEPTH];/*MAG_DEPTH is define in includes.h*/

#endif

