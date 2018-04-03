#include "includes.h"

#ifndef BANGBANG
#define BANGBANG
#define GRAV -.098
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

/*The struct below is essentially a wrapper struct used to track an 
 * the expanding radius to model the cloud of pellets being used as missle
 * defense by the boats*/
typedef struct
{
	Proj2Vec2f proj;
	float r;
} Def_proj;

void drawTraj(const Proj2Vec2f * shell);
void drawProj(const Proj2Vec2f * shell);
void drawDefProj(const Def_proj * dp);
void updateProj(Proj2Vec2f * shell);
void updateDefProj(Def_proj * dp);
bool detectIntercept(const Def_proj * dp, const Proj2Vec2f * shell);

extern Proj2Vec2f *mag[MAG_DEPTH];/*MAG_DEPTH is define in includes.h*/

#endif

