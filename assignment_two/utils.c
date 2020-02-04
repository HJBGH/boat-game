#include "includes.h"
#include "projectiles.h" //need vec3f

void drawVector(float x, float y, float z, float a, float b, float c, float s, bool n)
{
    if(n)
    {
        float h = fabs(sqrt((a*a)+(b*b)+(c*c)));
        a /= h;
        b /= h;
        c /= h;
    }
    glDisable(GL_LIGHTING); 
    glBegin(GL_LINES); 
    glVertex3f(x, y, z);
    glVertex3f(x+(a * s), y+(b * s), z+(c * s));
    glEnd();
    glEnable(GL_LIGHTING);
}

/*Highly derivative of the code found in the sample textureViewer program*/
unsigned int loadTexture(const char * fname)
{
    unsigned int tex_id = 
    SOIL_load_OGL_texture(fname, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                          SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    if(!tex_id) return 0; //load failure, this is caught in main.

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex_id;
}

/*this is a convienince function for drawing quads 
 * specified by float array arguement, textured with the
 * texture arguement, this gets used for the skybox and for
 * billboarding boat death sprites. This function deliberately avoids
 * setting lighting, materials, etc so that I can do that in the 
 * calling function*/
//t_id -> texture identity
void drawTexturedQuad(Vec3f p[], unsigned int t_id)
{
    /*enable texture*/
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t_id);
    /*Calling the functions below gets rid of seams*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /*Draw the quad*/
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(1,1);
    glVertex3f(p[0].x, p[0].y, p[0].z);
    glTexCoord2f(0, 1);
    glVertex3f(p[1].x, p[1].y, p[1].z);
    glTexCoord2f(0, 0);
    glVertex3f(p[2].x, p[2].y, p[2].z);
    //drawn as two triangles to avoid seams
    glTexCoord2f(0, 0);
    glVertex3f(p[2].x, p[2].y, p[2].z);
    glTexCoord2f(1, 0);
    glVertex3f(p[3].x, p[3].y, p[3].z);
    glTexCoord2f(1,1);
    glVertex3f(p[0].x, p[0].y, p[0].z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
}


void drawAxes(float l)
{
    //Need to do lighing status checks in here
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    
    glColor3f(1.0,0,0);
    glVertex3f(l,0,0);
    glVertex3f(0,0,0);
    glColor3f(0,1.0,0);
    glVertex3f(0,l,0);
    glVertex3f(0,0,0);
    glColor3f(0,0,1.0);
    glVertex3f(0,0,l);
    glVertex3f(0,0,0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void crossProduct(Vec3f *u, Vec3f *v, Vec3f *s)
{
    s->x = (u->y * v->z) - (u->z * v->y);
    s->y = (u->z * v->x) - (u->x * v->z);
    s->z = (u->x * v->y) - (u->y * v->x);
}

/*if dvs is true, calculate derivates*/
void calcSineWave(SineWave * sw, float x, float z, float t, float * y, 
bool dvs, float *dydx, float *dydz)
{
    float theta = (sw->kx * x) + (sw->kz * z) + (sw->wlen * t);
    *y = sw->amp * sinf(theta);
    if(dvs)
    {
        *dydz = (sw->kz * sw->amp) * cosf(theta);
        *dydx = (sw->kx * sw->amp) * cosf(theta);
    } 
}

/*l is the length of the waves array*/
/*It'd be better to zero out the old values in here*/
void sumWaves(SineWave * waves, int l, float x, float z, float t, 
float * total_y, bool dvs, float *total_dydx, float * total_dydz)
{
    *total_y = 0;
    *total_dydx = 0;
    *total_dydz = 0;
    int i = 0;
    float y = 0;
    float dydx = 0;
    float dydz = 0;
    while(i < l)
    {
        calcSineWave(waves, x, z, t, &y, dvs, &dydx, &dydz);
        *total_y += y;
        if(dvs)
        {
            *total_dydx += dydx;
            *total_dydz += dydz;
        }
        waves++;
        i++;
    }
}

/*draws a cyinder with it's lower end centered at the local 0,0*/
/*This function exists because I don't want to screw around with
 * glu built in functions*/
void hbDrawCylinder(float l, float r, int slices)
{
   //draw triangle fan for one end
    glEnable(GL_LIGHTING);
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0,-1,0);//fudge an initial normal
    glVertex3f(0,0,0);
    float i;
    float stepSize = (2*M_PI)/slices;
    float h;

    float x = 0;
    float z = 0;
    //Vec3f n; //normal vector
    for(i = 0; i < 2*M_PI; i += stepSize)
    {
        h = sqrt((x*x) + (z*z));
        x = cosf(i) * r;
        z = sinf(i) * r;
        glNormal3f(x/h, 0/h, z/h);//fudged normals
        glVertex3f(x, 0, z);
    }
    glEnd();

    //draw quad strip for middle bit
    x = 0;
    z = 0;
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i < 2*M_PI; i += stepSize)
    {
        x = cosf(i) * r;
        z = sinf(i) * r;
        h = sqrt((x*x) + (z*z));
        glNormal3f(x/h, 0/h, z/h); 
        glVertex3f(x, 0, z);

        glNormal3f(x/h, 0/h, z/h);
        glVertex3f(x, l, z);
    } 
    glEnd();

    /*draw triangle fan for other end*/
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);//fudge another initial normal
    glVertex3f(0,l,0);
    x = 0;
    z = 0;
    for(i = 0; i < 2*M_PI; i += stepSize)
    {
        x = cosf(i) * r;
        z = sinf(i) * r;
        h = sqrt((x*x) + (l*l) + (z*z));
        /*
        h = sqrt((x*x) + 1 + (z*z));
        Vec3f vector = {x, 0, z};
        Vec3f lastVector = {lastX, 0, lastZ};
        crossProduct(&vector, &lastVector, &n);
        printf("Normal x: %f, y: %f, z: %f\n", n.x, n.y, n.z);
        glNormal3f(n.x/h, n.y/h, n.z/h); */
        glNormal3f(x/h, l/h, z/h);
        glVertex3f(x, l, z);
    }
    glEnd();
    glDisable(GL_LIGHTING);
}

/*function of convenience, this function actually isn't that helpful*/
void hbDrawNormalAndVert3f(float x, float y, float z)
{   
    //I need to put visualizaiton in here I think
    float h = sqrt((x*x) + (y*y) + (z*z));
    glNormal3f(x/h, y/h, z/h);
    glVertex3f(x,y,z);
}


