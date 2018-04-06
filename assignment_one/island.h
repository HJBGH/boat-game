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
    Proj2Vec2f * shellp;/*regular cannon shell*/
    Proj2Vec2f * dp;/*Missile cannon shell*/
} Island;

#define ISLAND_GUN_L .15 /*island gun length*/
#define HEIGHT_OVER_X .2 /*how far the island protrudes over the x axis*/
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
bool detectIslandHit(const Proj2Vec2f * shell);
