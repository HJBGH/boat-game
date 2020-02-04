#include "includes.h"
#include "terrain.h"

//placeholder function while I experiment with finding bezier curve values
void drawTerrain(float size)
{
    glColor4f(.75,.7,.5, 1);
    GLfloat dirt[] = {.75, .7, .5, 1};
    GLfloat spec[] = {.5, .5, .5, .5};
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, dirt);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);//TODO: constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec); 

    if(wave_wire_flag)
    {
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }    

    float xStep = size/T_SEGMENTS;
    float zStep = size/T_SEGMENTS;
    glNormal3f(0, 1, 0);
    for(int j = 0; j <= T_SEGMENTS; j++)
    {
        float z = -(size/2.0) + j * zStep;
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i <= T_SEGMENTS; i++)
        { 
            float x = -(size/2.0) + i * xStep;
            //remember, t is just some ratio
            
            glVertex3f(x, OCEAN_FLOOR, z);
            glVertex3f(x,OCEAN_FLOOR,z+zStep);
        }
        glEnd();
    }
    //that's all.
}

    
