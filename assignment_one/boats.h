#include "includes.h"
/*Rotate by r, scale by s*/
#ifndef BOAT
#define MAX_ELEV 180
#define MIN_ELEV 0
#define BOAT
typedef struct
{
	bool left;/*used when programming control behaviour*/
	int health;
	float x; /*y gets calculated later, it doesn't have to be stored*/
	float gun_elev; /*gun elevation in degrees*/
	float colors[3];
} Boat;

void drawBoat(const Boat * boot, float s);
#endif
