#include "includes.h"
#ifndef ISLAND
#define ISLAND
typedef struct 
{
	int hp; 
	float gun_elev;	
	float gun_rot_s;/*gun rotation speed*/
} Island;

extern Island tasmania; /*find better name*/
#endif
void drawIsland(const Island * i); /*no scaling or custom co-ordinates needed*/
