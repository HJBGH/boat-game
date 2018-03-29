#include "includes.h"
#include "projectiles.h"
#ifndef ISLAND
#define ISLAND
typedef struct 
{
	int hp; 
	float gun_elev;	
	float gun_rot_s;/*gun rotation speed*/
	float cd; /*cooldown*/
	Proj2Vec2f * shellp;
} Island;

#define ISLAND_GUN_L .15 /*island gun length*/
#define HEIGHT_OVER_X .2 /*how far the island protrudes over the x axis*/
extern Island tasmania; /*find better name*/
#endif
void drawIsland(const Island * i); /*no scaling or custom co-ordinates needed*/
