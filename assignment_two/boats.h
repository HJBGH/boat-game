#include "includes.h"
#include "projectiles.h"
#include "turrets.h"
#ifndef BOAT
#define BOAT
#define MAX_ELEV 180
#define MIN_ELEV 0
#define BOAT_GUN_L .5
typedef struct
{
    bool alive;
    int hp;
    float heading; /* 0-359*/
    float s; /*s for SPEED, gotta go FAST*/
    float x; /*y gets calculated later, I've re-thought this,
                y gets used so frequently that it might be worth
                storing it somewhere*/
    float z; //candidate for refactoring into a vec2f
    float colors[3];
    Turret bt; /*bt for boat turret*/
} Boat;

extern Boat leftBoat;
extern Boat rightBoat;

void drawBoat(const Boat * boot, float s);
void updateBoat(Boat * boot);
/*used to update the boat position while it's moving*/
void updateBoatShell(const Boat * boot); 
/*used to recalculate shell position while it's loaded in a boat and the boat
 * is moving around, wrapper for updating through turret struct*/
bool detectBoatHit(const Boat * boot, const Proj2Vec3f * shell);
/* managing the consequences of a hit is performed in assignment_one.c, in
 * the update function*/
#endif
