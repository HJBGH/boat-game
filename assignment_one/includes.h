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
#define SEGMENTS 100
#define L_MAX -1.0
#define R_MAX 1.0
#define CENTER 0
#define AMP .2
#define WL 1
#define M_PI 3.14159265358979323846264338327
#endif

//maybe move these externs
extern bool wave_wire_flag;
extern bool wave_norm_flag;
extern bool wave_tang_flag;
/*I'll probably be able to store extern structs in here, which means it's a 
 * good place to declare Global*/
#ifndef GLOBAL
#define GLOBAL
typedef struct {float t, lastT, dt;} Global;
extern Global g;
#endif

void drawAxes(float l);
