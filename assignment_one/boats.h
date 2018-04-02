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
	float x; /*y gets calculated later, it doesn't have to be stored*/
	float gun_elev; /*gun elevation in degrees*/
	float colors[3];
    float s; /*s for SPEED, gotta go FAST*/
	float gun_rot_s; /*gun rotation speed*/
	float cd; /*cooldown*/
	float def_cd; /*missile defence cd*/
	Proj2Vec2f * shellp;
} Boat;

extern Boat leftBoat;
extern Boat rightBoat;

void drawBoat(const Boat * boot, float s);
void updateBoatShell(const Boat * boot); 
/*used to recalculate shell position while it's loaded in a boat and the boat
 * is moving around*/
#endif
