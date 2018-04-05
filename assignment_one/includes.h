#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#if _WIN32
#	include <Windows.h>
#endif
#if __APPLE__
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#	include <GLUT/glut.h>
#else 
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#ifndef WORLD_PROPERTIES
#define WORLD_PROPERTIES
#define OCEAN_FLOOR -1
//segments will eventually have to be a var not a def in order to facilitate
//wave resolution modification
#define L_MAX -1.0
#define R_MAX 1.0
#define CENTER 0
#define AMP .2
#define WL 1
#define M_PI 3.14159265358979323846264338327
/*MAX_T and MIN_T are limits on the tesselation of the waves*/
#define MAX_T 256
#define MIN_T 4
#define MAG_DEPTH 16 /*this is an arbitrarily chosen limit on the array of
projectile pointers, MAG meaning magazine*/
#define SHELL_S .275 /*muzzle velocity of projectiles*/
#define BOAT_SCALE .1 /*how boats are scaled*/
#define GUN_S 30 /*gun rotation speed*/
#define BOAT_SPEED .3 /*slow boats*/
/*gun elevation limits*/
#define MAX_I_ELEV 180
#define MAX_B_ELEV 90
#define MIN_ELEV 0

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
} Global;
extern Global g;
#endif

void drawAxes(float l);/*this gets used in drawboats() in boats.c*/
