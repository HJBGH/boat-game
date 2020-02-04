#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "SOIL.h"
#include <time.h>

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else 
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#ifndef WORLD_PROPERTIES
#define WORLD_PROPERTIES
#define OCEAN_FLOOR -3
#define WAVES 1

//limits for the x and z dim, to get the lower limit the constants are negated
#define Z_MAX 3.0
#define X_MAX 3.0
#define CENTER 0

/*MAX_T and MIN_T are limits on the tesselation of the waves*/
#define MAX_T 256
#define MIN_T 4

#define MAG_DEPTH 128 /*this is an arbitrarily chosen limit on the array of
projectile pointers, MAG meaning magazine*/
#define SHELL_S .75 /*muzzle velocity of projectiles*/
#define BOAT_SCALE .1 /*how boats are scaled*/
#define TURRET_SCALE .2
#define GUN_S 30 /*gun rotation speed*/
#define BOAT_SPEED .3 /*slow boats*/
/*gun elevation limits*/
#define MAX_I_ELEV 90
#define MAX_B_ELEV 90
#define MIN_ELEV 0
#define MAX_BOAT_POP 40

#endif

extern bool wave_wire_flag;
extern bool wave_norm_flag;
extern bool wave_tang_flag;
extern bool update_time_flag;
extern int segments;
extern float k;
#ifndef GLOBAL
#define GLOBAL
typedef struct {
    float t, lastT, dt, wt; 
    /* wt (wave time), this var is will have the value if t when 
     * update_time_flag is set to true. it's used to implement wave animation
     * pausing*/
    /*framerate measurement variables*/
    int frames;
    float frameRate, frameRateI;
    float lastFrameRateT;
    /*game_over man*/
    bool game_over;
    int boat_cd; //cooldown between spawning boats
} Global;
extern Global g;
#endif
#ifndef SINE_WAVE
#define SINE_WAVE
typedef struct
{
    float amp, kx, kz, wlen;
} SineWave;
#endif
#ifndef VEC3F
#define VEC3F
typedef struct
{
    float x,y,z;
} Vec3f;
#endif
#ifndef CAMERA
#define CAMERA
typedef struct{float x, y, lastx, lasty;} Camera;
extern Camera C;
#endif

//these get defined in utils.c
void drawAxes(float);
void drawVector(float, float, float, float, float, float, float, bool);
void crossProduct(Vec3f *, Vec3f *, Vec3f *);
void calcSineWave(SineWave * , float x, float z, float t, float * y, 
                    bool dvs, float *dydx, float *dydz);
void sumWaves(SineWave *,int l, float x, float z, float t, float * y, 
                    bool dvs, float *dydx, float *dydz);
void hbDrawCylinder(float, float, int);
void drawTexturedQuad(Vec3f p[], unsigned int t_id);
void hbDrawNormalAndVert3f(float x, float y, float z);
unsigned int loadTexture(const char * fname); //takes a filename as an argument
#ifndef BLEH
#define BLEH
/*wave array, needs to be accessed globally (boats)*/
extern SineWave Waves[]; /*{{.25, M_PI, M_PI * .1, .5 * M_PI},
                    {.25, M_PI, M_PI * 2,  M_PI * .25},
                    {.1, 0, M_PI * .5, M_PI * .25}};*/

#endif
