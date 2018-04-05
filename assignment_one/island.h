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
	float def_cd; /*missile defence cooldown*/
	Proj2Vec2f * shellp;
	Def_proj * dp;
} Island;

#define ISLAND_GUN_L .15 /*island gun length*/
#define HEIGHT_OVER_X .2 /*how far the island protrudes over the x axis*/
#define ISLAND_X .15 /*Half of the island's total width, used in hit detection,
drawing the island and limiting boat movement*/
extern Island tasmania; /*find better name*/
#endif
void drawIsland(const Island * i); /*no scaling or custom co-ordinates needed*/
void updateIsland(Island * i); /*used to update gun orientation*/
void updateIslandShell(const Island * i); 
/*Helper function used for updating the position of a loaded sell
 * when the island's cannon is moved */
bool detectIslandHit(const Proj2Vec2f * shell); 
/* Used to decrement the islands health and recycle the shell if it hits the 
 * island */
