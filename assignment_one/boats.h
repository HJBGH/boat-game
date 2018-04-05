#include "includes.h"
#include "projectiles.h"
/*Rotate by r, scale by s*/
#ifndef BOAT
#define BOAT
#define MAX_ELEV 180
#define MIN_ELEV 0
#define BOAT_GUN_L .5
typedef struct
{
	bool left;/*used drawBoat function*/
	int hp;
	float x; /*y gets calculated later, I've re-thought this,
				y gets used so frequently that it might be worth
				storing it somewhere*/
	float gun_elev; /*gun elevation in degrees*/
	float colors[3];
    float s; /*s for SPEED, gotta go FAST*/
	float gun_rot_s; /*gun rotation speed*/
	float cd; /*cooldown*/
	float def_cd; /*missile defence cd*/
	Proj2Vec2f * shellp; /*currently loaded cannon shell*/
	Def_proj * dp; /*currently loaded defense shell*/
} Boat;

extern Boat leftBoat;
extern Boat rightBoat;

void drawBoat(const Boat * boot, float s);
void updateBoat(Boat * boot);
void updateBoatShell(const Boat * boot); 
/*used to recalculate shell position while it's loaded in a boat and the boat
 * is moving around*/
bool detectBoatHit(const Boat * boot, const Proj2Vec2f * shell);
/* managing the consequences of a hit is performed in assignment_one.c, in
 * the update function*/
#endif
