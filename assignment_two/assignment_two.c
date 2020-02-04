#include "includes.h"
#include "keyboard.h"
#include "mouse.h"
#include "boats.h"
#include "turrets.h"
#include "island.h"
#include "projectiles.h"
#include "terrain.h"

#define BOAT_HP 1
#define TAS_HP 100

Camera C = {0, 0, 0, 0};

SineWave Waves[] = {{.1, M_PI, M_PI * .5, .5 * M_PI},
                    {.2, M_PI * .7, M_PI * .9,  M_PI * .25},
                    {.15, 0, M_PI * .5, M_PI * .5}};
                    
//SineWave Waves[] = {{.0325, M_PI, M_PI * .1, .5 * M_PI}};

/*initialize the global flags that were declared in includes.h*/
bool axis_flag = false;
bool wave_wire_flag = false;
bool wave_norm_flag = false;
bool wave_tang_flag = false;
bool update_time_flag = true;
int segments = 32;

/*skybox texture ids, there's probably a better way to do this.*/
unsigned int front_texture = 0;
unsigned int left_texture = 0;
unsigned int back_texture = 0;
unsigned int right_texture = 0;
unsigned int down_texture = 0;
unsigned int up_texture = 0;
unsigned int p_texture = 0; //This will be a particle texture


Proj2Vec3f *mag[MAG_DEPTH];
Proj2Vec3f *def_mag[MAG_DEPTH];/*this second array of projectile pointers 
are used for modeling the flight of the defensive pellets*/
Boat *boats[MAX_BOAT_POP];
//the two variables below are used for initializing the boats
float boat_color[3] = {1, 0, 0};
Turret base_bt = {0, 0, 45, 45, 0, 0, 0, 0, NULL, NULL};
int score = 0;
Global g =
{
    .frameRateI = 1,
    .game_over = false
};

/*prototype for a helper function, it's at the very end of the file*/
void boatCDhelper(Boat * boot);
void drawOSD();

Island tasmania = 
{
    .hp = TAS_HP,
    .it = {0, 0, 45, 0, 0, 0, 0, 0, NULL, NULL}
};

/* used for debugging
Boat leftBoat = 
{
    .heading = 90,
    .hp = BOAT_HP,
    .x = -.5,
    .z = 0,
    .colors = {1, 0, 0},
    .s = 0,
    .bt = {0, 0, 45, 45, 0, 0, 0, 0, NULL, NULL}
};*/

/* I wasn't sure where exactly to put game logic, so it's mostly 
 * in the idle loop*/
void idle()
{
    /*Timing updates*/
    g.lastT = g.t;
    g.t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    if(update_time_flag) g.wt = g.t;
    g.dt = g.t - g.lastT; 
    if(g.boat_cd > 0)
    {
        g.boat_cd -= g.dt;
        if(g.boat_cd < 0) g.boat_cd = 0;
    }
    /*cooldown management for the island*/
    if(tasmania.it.cd > 0)
    {
        /*this is not a robust way of doing things*/
        tasmania.it.cd -= g.dt;
        if(tasmania.it.cd < 0)
        {
            tasmania.it.cd = 0;
        }
    }   
    if(tasmania.it.def_cd > 0)
    {
        tasmania.it.def_cd -= g.dt;
        if(tasmania.it.def_cd < 0)
        {
            tasmania.it.def_cd = 0;
        }
    }
    if(tasmania.it.cd == 0 && tasmania.it.shellp == NULL)
    {
        /* find a free projectile */
        /* this is somewhat precarious, if no free projectile
         * can be found there'll be problems.*/
        for(int i = 0; i < MAG_DEPTH; i++)
        {
            if(mag[i]->loaded == false && mag[i]->fired == false)
            {
                mag[i]->loaded = true;
                tasmania.it.shellp = mag[i];
                printf("new shell loaded into island cannon\n");
                break;
            }
        }
    }
    /*identical behaviour to the above statement, except for Def_projs*/
    if(tasmania.it.def_cd == 0 && tasmania.it.dp == NULL)
    {
        for(int i = 0; i < MAG_DEPTH; i++)
        {
            if(def_mag[i]->loaded == false 
            && def_mag[i]->fired == false)
            {
                def_mag[i]->loaded = true;
                tasmania.it.dp = def_mag[i];
                tasmania.it.dp->r = .01;/*set initial radius*/
                printf("new shell loaded into island cannon\n");
                break;
            }
        }
    }
    /*reload boat shells*/
    /* these update methods are contained in their respective files, they
     * allow for position / rotation updates independent of g.dt*/
    updateIsland(&tasmania);
    /* these update methods are contained in their respective 
     * files, they essentially allow the shell to be moved with the
     * movement of the boat's gun as it bobs on the waves*/
    updateIslandShell(&tasmania);
    int i;
    for(i = 0; i < MAX_BOAT_POP; i++)
    {
        updateBoat(boats[i]);
    }
    
    for(int i = 0; i < MAG_DEPTH; i++)
    {
        if(mag[i]->fired == true)
        {
            /*also need to do hit detection in here*/
            updateProj(mag[i]);
            if(detectIslandHit(mag[i]))
            {
                tasmania.hp--;
                mag[i]->fired = false;
                mag[i]->loaded = false;
            } 
            /*O(N^2), absolute garbage fire, this isn't good*/
            for(int k = 0; k < MAG_DEPTH; k++)
            {
                if(def_mag[k]->fired == true)
                {
                    if(detectIntercept(def_mag[k], mag[i]))
                    {
                        /*the shell has been intercepted, recycle it*/
                        mag[i]->fired = false;
                        mag[i]->loaded = false;
                    }
                }
            }
        }
        if(def_mag[i]->fired == true)
        {
            updateDefProj(def_mag[i]);
        }
    }

    if(tasmania.hp <= 0)
    {
        g.game_over = true;
    }
    /*Finally; update framerate tracking information*/
    float fdt = g.t - g.lastFrameRateT;
    if (fdt > g.frameRateI) 
    {
        g.frameRate = g.frames / fdt;
        g.lastFrameRateT = g.t;
        g.frames = 0;
    }
    /*find a boat, set it's start, cannon angles etc*/
    if(g.boat_cd == 0)
    {
        //linear search, not good
        for(int i =0; i< MAX_BOAT_POP; i++)
        {
            //if the boat isn't currently running
            if(!boats[i]->alive)
            {
                
                boats[i]->alive = true;
                printf("%d, randmax\n", RAND_MAX);
                int r = rand();
                if(r%2 ==0)
                {
                    //set boat up on x
                    boats[i]->z = Z_MAX;
                    //generate random x
                }
                else
                {
                    //set boat up on z
                    
                }
            }
        }
    }
    glutPostRedisplay();
}

/*draw a vector with it's origin at x,y to <a,b> scaled by s and normalized
 * if n is true. Color this vector according to cr, cg, and cb*/

/*should this take arguments? who knows.*/
void drawOcean(float size, int segments) 
{
    float y = 0;
    float dyZ = 0;
    float dyX = 0;
    float xStep = size/segments;
    float zStep = size/segments;
    Vec3f result;
    for(int j = 0; j <= segments; j++)
    {
        float z = -(size/2.0) + j * zStep;
        for(int i = 0; i <= segments; i++)
        {
            //learn to use partial derivatives
            /*This calculation gives the points for each
            * part of the line/plane being drawn*/
            float x = -(size/2.0) + i * xStep;
            /*zero out old vars*/
            y = 0;
            dyZ = 0;
            dyX = 0;
            
            sumWaves(Waves, WAVES, x, z, g.wt, &y, true, &dyX, &dyZ);
            Vec3f dyZvector = {0, dyZ, 1};
            Vec3f dyXvector = {1, dyX, 0};
            crossProduct(&dyZvector, &dyXvector, &result);
            //normals
            if(wave_tang_flag)//draw bi-normal and tangent
            {
                glColor3f(1,0,1);
                //bi-normal
                drawVector(x, y, z, 0, dyZ, 1, .1, true);
                glColor3f(0,1,1);
                //tangent
                drawVector(x, y, z, 1, dyX, 0, .1, true);
            }
            if(wave_norm_flag) //draw normal
            { 
                glColor3f(1,1,0);
                drawVector(x, y, z, result.x, result.y, result.z, .1, true);
            }
        }
    }
    
    glColor4f(0,.7,1.0,.45);
    GLfloat cyan[] = {0.0, .7, 1.0, .5};
    GLfloat spec[] = {1.0, 1.0, 1.0, 1};
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100);//TODO: constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec); 

    if(wave_wire_flag)
    {
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }    
    glNormal3f(0,0,1);
    for(int j = 0; j <= segments; j++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        float z = -(size/2.0) + j * zStep;
        for(int i = 0; i<=segments; i++)
        {
            /*This calculation gives the points for each
            * part of the line/plane being drawn*/
            float x = -(size/2.0) + i * xStep;
            y = 0;
            dyX = 0;
            dyZ = 0;
            sumWaves(Waves, WAVES, x, z, g.wt, &y, true, &dyX, &dyZ); 

            float h = sqrt((dyX*dyX)+1+(dyZ*dyZ));
            Vec3f dyZvector = {0, dyZ, 1};
            Vec3f dyXvector = {1, dyX, 0};
            crossProduct(&dyZvector, &dyXvector, &result);
            //for some reason the lighting isn't working
            glNormal3f(result.x/h, result.y/h, result.z/h);
            glVertex3f(x,y,z);
            //Do the second one
            y = 0;
            dyX = 0;
            dyZ = 0;
            sumWaves(Waves, WAVES, x, z+zStep, g.wt, &y, true, &dyX, &dyZ);
            h = sqrt((dyX*dyX)+1+(dyZ*dyZ));
            //I could've just reused the previous vectors
            Vec3f dyZvector2 = {0, dyZ, 1};
            Vec3f dyXvector2 = {1, dyX, 0};

            crossProduct(&dyZvector2, &dyXvector2, &result);
            glNormal3f(result.x/h, result.y/h, result.z/h);
            glVertex3f(x,y,z+zStep);
        }
        glEnd(); 
    }
    //reset the poly mode, in case wireframe is on
    glPolygonMode(GL_FRONT, GL_FILL);
    return;
}

/*Textures get loaded as part of init()*/
void drawSkybox()
{
    //step one, construct a cube with its faces inwards

    glDisable(GL_DEPTH_TEST);
    glColor4f(.75,.75,.95,1);
    GLfloat gray[] = {.5,.5,.5,1};
    GLfloat spec[] = {0.0, 0.0, 0.0, 1};
    glDisable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
    //glMaterialf(GL_FRONT, GL_SHININESS, 100);//TODO: constant
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
    if(wave_wire_flag)
    {
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }  
    /*I'm aware that the texture inherits the color of the material,
     * I've left it like this because I think it looks good*/
    //C shape winding
    //draw front
    Vec3f front[] = {{8, 8, -8},{-8, 8, -8},{-8, -8, -8},{8, -8, -8}};
    drawTexturedQuad(front, front_texture);
    //draw left side
    Vec3f left[] = {{-8, 8, -8}, {-8, 8, 8}, {-8, -8, 8}, {-8, -8, -8}};
    drawTexturedQuad(left, right_texture); 
    //at some point I mixed the left and right textures up and I haven't 
    //bothered to change them back
    //draw right
    Vec3f right[] = {{8, 8, 8}, {8, 8, -8}, {8, -8, -8}, {8, -8, 8}};
    drawTexturedQuad(right, left_texture);  
    //draw back
    Vec3f back[] = {{-8, 8, 8},{8, 8, 8},{8, -8, 8},{-8, -8, 8}};
    drawTexturedQuad(back, back_texture);
    //draw up
    Vec3f up[] = {{8,8,8},{-8,8,8},{-8,8,-8},{8,8,-8}};
    drawTexturedQuad(up, up_texture);
    Vec3f down[] = {{8,-8,-8},{-8,-8,-8},{-8,-8,8},{8,-8,8}};
    drawTexturedQuad(down, down_texture);
    glEnable(GL_DEPTH_TEST);
}



void display()
{
    int err;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLfloat p[] = {4.0, 7.0, 7.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, p);
    //
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    /*C is the camera, check includes.h*/
    glRotatef(C.y, 1, 0, 0);
    glRotatef(C.x, 0, 1, 0);

    drawSkybox();
     /*Do game over testing before we draw anything*/
    if(g.game_over)
    {
        drawOSD();
        glutSwapBuffers();
        return;
    }
    drawAxes(1.0);
    glPushMatrix();
    if (axis_flag) {drawAxes(.3);}
    drawIsland(&tasmania); 
    int i;
    for(i = 0; i < MAX_BOAT_POP; i++)
    {
        drawBoat(boats[i], BOAT_SCALE);
    }
    /*draw trajectories, draw projectiles, draw defense, loop through mags*/
    
    for(i = 0; i < MAG_DEPTH; i++)
    {
        if(mag[i]->loaded == true ||mag[i]->fired == true)
        {
            drawTraj((mag[i]));
        }
        if(mag[i]->fired == true)
        {
            drawProj((mag[i]));
        }
        if(def_mag[i]->fired == true)
        {
            drawProj(def_mag[i]);
        }
    }
    drawTerrain(Z_MAX * X_MAX);
    drawOcean(Z_MAX * X_MAX, segments);
    if((err = glGetError()))
    {
        printf("%s: %x\n", (gluErrorString(err)), err);
    }
    g.frames++;
    glPopMatrix();
    drawOSD();
    glPopMatrix();
    glutSwapBuffers();
}

void init()
{
    /*malloc projectile array in here, these actually never get cleaned up
     * properly because GLUT doesn't allow one to exit the main loop afaik and
     * I'm weary of doing calls to free() in idle or something*/
    srand(time(NULL));
    int i;
    for(i = 0; i < MAG_DEPTH; i++)
    {
        mag[i] = (Proj2Vec3f*) malloc(sizeof(Proj2Vec3f));
        printf("Projectile memory allocated\n");
        mag[i]->fired = false;
        mag[i]->loaded = false;
        mag[i]->r = .01; //very small radius for cannon balls, doesn't change
        def_mag[i] = (Proj2Vec3f*) malloc(sizeof(Proj2Vec3f));
        printf("Defensive projectile memory allocated\n");
        def_mag[i]->fired = false;
        def_mag[i]->loaded = false;
        def_mag[i]->r = .01;
    }
    
    //init boats 
    printf("trying to initialize the boats\n");
    float f;
    Turret bt;
    i = 0;
    for(f = 0; f < 360 && i < MAX_BOAT_POP; f+= 360/MAX_BOAT_POP)
    {
        /*leave x,z co-ord as garbage for now*/
        boats[i] = (Boat*) malloc(sizeof(Boat));
        boats[i]->hp = BOAT_HP;
        boats[i]->heading = 90;
        boats[i]->s = BOAT_SPEED;
        boats[i]->x = cosf((f * M_PI) / 180) * 2;
        boats[i]->z = -sinf((f * M_PI) / 180) * 2;
        boats[i]->alive = true;
        boats[i]->colors[0] = boat_color[0];
        boats[i]->colors[1] = boat_color[1];
        boats[i]->colors[2] = boat_color[2];
        //I can't work out how to copy one struct into another gracefully.
        bt = boats[i]->bt; 
        bt.x = 0;
        bt.z = 0;
        bt.gun_elev = 0;
        bt.facing = 0;
        bt.rot_s = 0;
        bt.cd = 0;
        bt.def_cd = 0;
        bt.shellp = NULL;
        bt.dp = NULL;
        //I can work out the desired heading based on these co-ordinates
        i++;
    }
    printf("Boats initialized\n");
    /* In this program these OpenGL calls only need to be done once,
      but normally they would go elsewhere, e.g. display */
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    //no error checking, bad practise
    front_texture = loadTexture("./textures/TropicalSunnyDayFront2048.png");
    left_texture = loadTexture("./textures/TropicalSunnyDayLeft2048.png");
    right_texture = loadTexture("./textures/TropicalSunnyDayRight2048.png");
    back_texture = loadTexture("./textures/TropicalSunnyDayBack2048.png");
    up_texture = loadTexture("./textures/TropicalSunnyDayUp2048.png");
    down_texture = loadTexture("./textures/TropicalSunnyDayDown2048.png");
}

void reshape(int w, int h)
{
    printf("Window width %d\n", w);
    printf("Window height %d\n", h);
    glViewport(0,0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, (double)w/(double)h, 0.01, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(600, 600);
    glutCreateWindow("Island defence 3D");

    init();
   
    printf("initialization completed\n");
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseClicked); 
    glutReshapeFunc(reshape);
    glutMainLoop();

    return EXIT_SUCCESS;
}

//TODO:use a cannon array to iterate through cannons and reset appropriate vars
/*the function below is a consequence of a badly thought out implementation
 * of the cannons*/
void boatCDhelper(Boat * boot)
{
    if(boot->bt.cd > 0)
    {
        /*this is not a robust way of doing this*/
        boot->bt.cd -= g.dt;
        if(boot->bt.cd < 0)
        {
            boot->bt.cd = 0;
        }
    }
    /*
    if(boot->def_cd > 0)
    {
        boot->def_cd -= g.dt;
        if(boot->cd < 0)
        {
            boot->cd = 0;
        }
    }*/
    if(boot->bt.cd == 0 && boot->bt.shellp == NULL)
    {
        for(int i = 0; i < MAG_DEPTH; i++)
        {
            if(mag[i]->loaded == false && mag[i]->fired == false)
            {
                mag[i]->loaded = true;
                boot->bt.shellp = mag[i];
                break;
            }
        }
    }
    /*
    if(boot->dp == NULL && boot->def_cd <= 0)
    {
        for(int i = 0; i < MAG_DEPTH; i++)
        {
            if(def_mag[i]->loaded == false 
            && def_mag[i]->fired == false)
            {
                def_mag[i]->loaded = true;
                boot->dp = def_mag[i];
                boot->dp->r = .01;
                break;
            }
        }
    }*/
   //TODO, remove this damn function, it's a result of shitty code
}

/*I'll be honest, this code is pretty much the same as the 
 * OSD from tute 3 with the exception of the health bars*/
void drawOSD()
{
    char char_buf[30];
    char * bufp;
    int w, h;
    
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    /* Set up orthographic coordinate system to match the 
     window, i.e. (0,0)-(w,h) */
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);
    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /*Game over, only occurs if a ship or the island dies*/
    if(g.game_over)
    {
        glColor3f(1.0, .5, 0);
        glRasterPos2i(200, 250);
        snprintf(char_buf, sizeof char_buf, "GAME OVER");
        for (bufp = char_buf; *bufp; bufp++)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
        }
        /* Pop modelview */
        glPopMatrix();  
        glMatrixMode(GL_PROJECTION);

        /* Pop projection */
        glPopMatrix();  
        glMatrixMode(GL_MODELVIEW);

        /* Pop attributes */
        glPopAttrib();
        return;
    }
    

    /* Frame rate */
    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(350, 485);
    snprintf(char_buf, sizeof char_buf, "fr (f/s): %6.0f", g.frameRate);
    for (bufp = char_buf; *bufp; bufp++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    }

    /* Time per frame */
    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(350, 470);
    snprintf(char_buf, sizeof char_buf, "ft (ms/f): %5.0f", 
                                                1.0 / g.frameRate * 1000.0);
    for (bufp = char_buf; *bufp; bufp++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    }

    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(100, 485);
    snprintf(char_buf, sizeof char_buf, "Health: %d", 
                                                tasmania.hp);
    for (bufp = char_buf; *bufp; bufp++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    }
 
    /* Pop projection */
    /*hard coded health bar drawing, drawn as quads*/
    /* Pop modelview */
    glPopMatrix();  
    glMatrixMode(GL_PROJECTION);

    /* Pop projection */
    glPopMatrix();  
    glMatrixMode(GL_MODELVIEW);

    /* Pop attributes */
    glPopAttrib();
    //this doesn't work but I've left it in.
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-.9, .9, 0);
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex3f(0,.3,0);
    glVertex3f(0,.3,0);
    glVertex3f(tasmania.hp*.1, 5, 0);
    glVertex3f(tasmania.hp*.1, 4.5, 0);
    glEnd();

    glPopMatrix();

    return;
}
