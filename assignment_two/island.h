#include "includes.h"
#include "projectiles.h"
#include "turrets.h"

#ifndef ISLAND
#define ISLAND
typedef struct 
{
    int hp; 
    Turret it; /*it being island turret*/
} Island;

#define ISLAND_GUN_L .15 /*island gun length*/
#define HEIGHT 1.7
#define HEIGHT_OVER_X .2
#define ISLAND_X .15 /*Half of the island's total width, used in hit detection,
drawing the island and limiting boat movement*/
extern Island tasmania; /*give things meaningful names*/
#endif
void drawIsland(const Island * i); /*no scaling or custom co-ordinates needed*/
void updateIsland(Island * i); /*used to update gun orientation independent
                                 of fps*/
void updateIslandShell(const Island * i); 
/*Helper function used for updating the position of a loaded projectile
 * when the island's cannon is moved */
bool detectIslandHit(const Proj2Vec3f * shell);
