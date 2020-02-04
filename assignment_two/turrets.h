#include "includes.h"
#include "projectiles.h"

#ifndef TURRETS
#define TURRETS

#define T_HEIGHT .5
#define T_WIDTH .5
#define T_GUN_L .75
#define T_GUN_W .2

/*This is effectively a container struct to abstract common functionality 
 * of the boats and island into a single managable chunk so that I don't have
 * to implement it twice like i did for the last assignment*/
typedef struct
{
    float x;
    float z;
    /*x and y co-ords, useful for drawing and for projectile launching*/
    float gun_elev;
    float facing; /*rotation*/
    float gun_elev_s; /*speed of elevation/depression*/
    float rot_s; /*rotation speed*/
    float cd; /*regular cooldown*/
    float def_cd; /*defensive cooldown*/
    Proj2Vec3f * shellp;/*regular shell*/
    Proj2Vec3f * dp; /*defensive shell*/
} Turret;

void drawTurret(const Turret * turret, GLfloat * c);
void updateTurret(Turret * t, float min_elev, float max_elev);
void updateLoadedShell(const Turret * t);
#endif
