#include "includes.h"
void drawBoat();
#ifndef BOAT
#define MAX_ELEV 180
#define MIN_ELEV 0
#define BOAT
typedef struct
{
	bool left;/*used when programming control behaviour*/
	int health;
	int x; /*y gets calculated later, it doesn't have to be stored*/
	int gun_elev; /*gun elevation in degrees*/
	float colors[3];
} Boat;
#endif
