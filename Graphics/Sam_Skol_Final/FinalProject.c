// Project Final
// Rain and Snow Scene with heavy fog
// Sam Skolnekovich
// 11/6/16
//


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//obj files
#include "glm.h"

//rain
#define MAX_PARTICLES 10000
#define MAX_PARTICLESTWO 90000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2


int moreParticles=0;
float slowdown = 2.0;
float slowdownSnow=2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int help=1;

int maxGrass =36;
int minGrass=29;

int loop;
int fall=SNOW;

//floor colors
float accum = -10.0;

typedef struct {

  int alive;
  float life;	// particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

// Particle System
particles par_sys[MAX_PARTICLES];
particles par_sys2[MAX_PARTICLESTWO];


//dem file
float zmin=+1e8;       //  DEM lowest location
float zmax=-1e8;       //  DEM highest location
float zs[65][65];       //  DEM data
float zmag=.4;          //  DEM magnification
unsigned int texture[6];  //  Texture names

//fog constants
GLfloat density = 0.01; //set the density to 0.3 which is
GLfloat rainColor[4] = {0.42, 0.42, 0.42, 1.0}; //set the for
GLfloat snowColor[4] = {0.6, 0.6, 0.6, 1.0}; //set the for


//camera
float angle=0.0;
float lx=0.0f,lz=-1.0f, ly=0.0f;
float x=0.0f,z=30.0f, y=45.0f;
//normal
int axes=0;       //  Display axes
int mode=2;       //  Projection mode
int th=120;         //  Azimuth of view angle
int ph=30;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=222.0;   //  Size of world
int rev=0;

//  Macro for sin & cos in degrees
//camera
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text
//bool begun=false;


GLfloat gfPosY = 0.0;
GLfloat gfDeltaY = -.01;
int change_x;
int change_y;



#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

float getRandom(){
  return (float)rand() / (float)RAND_MAX ;
}



//CODE FOR DIFFERENT SPHERES

/*static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

static void VertexFun(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}*/


/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
/*static void sphereFun(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   VertexFun(0,-90);
   for (th=0;th<=360;th+=d)
   {
      VertexFun(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         VertexFun(th,ph);
         VertexFun(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   VertexFun(0,90);
   for (th=0;th<=360;th+=d)
   {
      VertexFun(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void sphere1(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}
*/


static void Grass(double xg, double yg, double zg){
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

  if(fall==RAIN){
glBindTexture(GL_TEXTURE_2D,texture[2]);
  }
  else{
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  }

GLMmodel * model;
model= glmReadOBJ("Grass/grass.obj");
glmFacetNormals(model);
glmVertexNormals(model,180.0);
glScaled(10,10,10);
glPushMatrix();
glTranslated(xg,yg,zg);
glmDraw(model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
glPopMatrix();

}

static void treeOne(double xg, double yg, double zg){
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
glBindTexture(GL_TEXTURE_2D,texture[1]);
GLMmodel * model;
model= glmReadOBJ("objs/tree.obj");
glmFacetNormals(model);
glmVertexNormals(model,180.0);
glScaled(20,20,20);
glPushMatrix();
glTranslated(xg,yg,zg);
glmDraw(model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
glPopMatrix();

}

static void treeTwo(double xg, double yg, double zg){
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
glBindTexture(GL_TEXTURE_2D,texture[0]);
GLMmodel * model;
model= glmReadOBJ("DeadTree/DeadTree.obj");
glmFacetNormals(model);
glmVertexNormals(model,180.0);
glScaled(30,40,30);
glPushMatrix();
glTranslated(xg,yg,zg);
glmDraw(model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
glPopMatrix();

}


static void treeThree(double xg, double yg, double zg){
glEnable(GL_TEXTURE_2D);
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
glBindTexture(GL_TEXTURE_2D,texture[1]);
GLMmodel * model;
model= glmReadOBJ("objs/Tree_Dry_1_Obj/Tree_Dry_1.obj");
glmFacetNormals(model);
glmVertexNormals(model,180.0);
glScaled(.05,.05,.05);
glPushMatrix();
glTranslated(xg,yg,zg);
glmDraw(model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
glPopMatrix();

}


void initParticles(int i) {
    par_sys[i].alive = 1;
    par_sys[i].life = 1.0;
    par_sys[i].fade = (getRandom()*120.0f)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 1100-620);
    par_sys[i].ypos = (getRandom()*100+5);
    par_sys[i].zpos = (float) (rand() % 1100-550);

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;//-0.8;

}

void initParticles2(int i) {
    par_sys2[i].alive = 1;
    par_sys2[i].life = 1.0;
    par_sys2[i].fade = (getRandom()*120.0f)/1000.0f+0.003f;

    par_sys2[i].xpos = (float) (rand() % 1100-620);
    par_sys2[i].ypos = (getRandom()*100+5);
    par_sys2[i].zpos = (float) (rand() % 1100-550);

    par_sys2[i].red = 0.5;
    par_sys2[i].green = 0.5;
    par_sys2[i].blue = 1.0;

    par_sys2[i].vel = velocity;
    par_sys2[i].gravity = -0.8;//-0.8;

}




void init (void) {

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    // Initialize particles
  if(moreParticles==0){
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }
  }
  else{
    for (loop = 0; loop < MAX_PARTICLESTWO; loop++) {
        initParticles2(loop);
    }
  }
  //fog
  if(fall==RAIN){
glEnable (GL_DEPTH_TEST); //enable the depth testing
glEnable (GL_FOG); //enable the fog
glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
glFogfv (GL_FOG_COLOR, rainColor); //set the fog color to
glFogf (GL_FOG_DENSITY, density); //set the density to the
glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the
  }

  else{
glEnable (GL_DEPTH_TEST); //enable the depth testing
glEnable (GL_FOG); //enable the fog
glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
glFogfv (GL_FOG_COLOR, snowColor); //set the fog color to
glFogf (GL_FOG_DENSITY, density); //set the density to the
glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the
  }

}



// For Rain
void drawRain() {
  float xr, yr, zr;
   if(moreParticles==0){
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == 1) {
      xr = par_sys[loop].xpos;
      yr = par_sys[loop].ypos;
      zr = par_sys[loop].zpos+zoom+20;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glPushMatrix();
      glTranslatef(xr, yr, zr);
      //sphere1(0,0,0,10);
      glutSolidSphere(0.08, 8, 8);
      glPopMatrix();

      // Update values
      //Move
      // Adjust slowdown
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*4000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].life = -1.0;
      }
      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
   }
  else{
    for (loop = 0; loop < MAX_PARTICLESTWO; loop=loop+2) {
    if (par_sys2[loop].alive == 1) {
      xr = par_sys2[loop].xpos;
      yr = par_sys2[loop].ypos;
      zr = par_sys2[loop].zpos+zoom+20;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glPushMatrix();
      glTranslatef(xr, yr, zr);
      //sphere1(0,0,0,10);
      glutSolidSphere(0.08, 8, 8);
      glPopMatrix();

      // Update values
      //Move
      // Adjust slowdown
      par_sys2[loop].ypos += par_sys2[loop].vel / (slowdown*4000);
      par_sys2[loop].vel += par_sys2[loop].gravity;
      // Decay
      par_sys2[loop].life -= par_sys2[loop].fade;

      if (par_sys2[loop].ypos <= -10) {
        par_sys2[loop].life = -1.0;
      }
      //Revive
      if (par_sys2[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
  }


}

// For Hail NOT USED
void drawHail() {
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == 1) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].vel = par_sys[loop].vel * -1.0;
      }
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*50); // * 1000
      par_sys[loop].vel += par_sys[loop].gravity;

      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}


// For Snow
void drawSnow() {
  float x, y, z;
  if(moreParticles==0){
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == 1) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
      glTranslatef(x, y, z);
      //sphere1(0,0,0,1);
      glutSolidSphere(0.2, 16, 16);
      glPopMatrix();

      // Update values
      //Move
      par_sys[loop].ypos += par_sys[loop].vel / (slowdownSnow*5);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;



      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
  }
  else{
  for (loop = 0; loop < MAX_PARTICLESTWO; loop=loop+2) {
    if (par_sys2[loop].alive == 1) {
      x = par_sys2[loop].xpos;
      y = par_sys2[loop].ypos;
      z = par_sys2[loop].zpos + zoom;

      // Draw particles
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
      glTranslatef(x, y, z);
      //sphere1(0,0,0,1);
      glutSolidSphere(0.2, 16, 16);
      glPopMatrix();

      // Update values
      //Move
      par_sys2[loop].ypos += par_sys2[loop].vel / (slowdownSnow*5);
      par_sys2[loop].vel += par_sys2[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys2[loop].fade;



      //Revive
      if (par_sys2[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
  }
}



void generateScene()
{
   int i,j;
  double z0 = (zmin+zmax)/2;
  int treeSelect=1;
   //  Apply texture to one large quad

      glColor3f(1,1,1);
      glEnable(GL_TEXTURE_2D);
  if(fall==RAIN){
      glBindTexture(GL_TEXTURE_2D,texture[4]);
  }
  else{
    glBindTexture(GL_TEXTURE_2D,texture[5]);
  }
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      for (i=0;i<64;i++)
         for (j=0;j<64;j++)
         {
            float xs=16*i-512;
            float ys=16*j-512;
            glBegin(GL_QUADS);
            glTexCoord2f((i+0)/64.,(j+0)/64.); glVertex3d(xs+ 0,ys+ 0,zmag*(zs[i+0][j+0]-z0));
            glTexCoord2f((i+1)/64.,(j+0)/64.); glVertex3d(xs+16,ys+ 0,zmag*(zs[i+1][j+0]-z0));
            glTexCoord2f((i+1)/64.,(j+1)/64.); glVertex3d(xs+16,ys+16,zmag*(zs[i+1][j+1]-z0));
            glTexCoord2f((i+0)/64.,(j+1)/64.); glVertex3d(xs+ 0,ys+16,zmag*(zs[i+0][j+1]-z0));
            glEnd();

         }

      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_TEXTURE_2D);


  // generate grass
       for (i=minGrass;i<maxGrass;i++){
         for (j=minGrass;j<maxGrass;j++)
         {
           float xs=16*i-512;
           float ys=16*j-512;
           float zsTwo=(zs[i][j]-z0)*zmag;
          //Grass Generation
          //front
           glPushMatrix();
           glTranslated(xs,ys,(zs[i][j]-z0)*zmag-4);
           glRotated(90,1,0,0);
           Grass(0,0,0);
           glPopMatrix();


          }

    }


  //generate trees
    for (i=1;i<64;i++){
         for (j=1;j<64;j++)
         {
           float xs=16*i-512;
           float ys=16*j-512;
           float zsTwo=(zs[i][j]-z0)*zmag;

          glPushMatrix();
          glTranslated(xs,ys,zsTwo);
          glRotated(90,1,0,0);
           if(treeSelect==1){
            treeOne(0,0,0);
           }
           if(treeSelect==2){
             treeTwo(0,0,0);
           }
           if(treeSelect==3){
             treeThree(0,0,0);
           }

           treeSelect+=1;
           if(treeSelect>3){
             treeSelect=1;
           }
           glPopMatrix();
           j+=20;
          }
    i+=20;
    }
     // glTranslated(500,100,(zs[32][32]-z0)*zmag);
}


void ReadDEM(char* file)
{
   int i,j;
   FILE* f = fopen(file,"r");
   if (!f) Fatal("Cannot open file %s\n",file);
   for (j=0;j<=64;j++)
      for (i=0;i<=64;i++)
      {
         if (fscanf(f,"%f",&zs[i][j])!=1) Fatal("Error reading saddleback.dem\n");
         if (zs[i][j] < zmin) zmin = zs[i][j];
         if (zs[i][j] > zmax) zmax = zs[i][j];
      }
   fclose(f);
}






/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/20,20*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}



void display()
{
   //  Clear screen


  glClearColor(0.4,0.4,.4,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //  Reset previous transforms
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_ALWAYS);
 // glDisable(GL_CULL_FACE);


  if (mode==1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   if(mode==2){
    gluLookAt( x, y, z,
        x+lx, y+ly, z+lz,
        0.0f, 1.0f, 0.0f);
   }
   //  Orthogonal - set world orientation

  if(mode==0)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }


  glPushMatrix();
    if (fall == RAIN) {
        drawRain();
      }else if (fall == HAIL) {
        drawHail();
      }else if (fall == SNOW) {
        drawSnow();
      }
    glPopMatrix();

   //  Set view angle
   //  Perspective - set eye position
    glPushMatrix();
    glRotated(-90,1,0,0);
    generateScene();
    glPopMatrix();

   glColor3f(1,.5,.5);
  //  Display parameters
   glWindowPos2i(5,5);

   //Print("%d,%d, %d",zmag,y,z);
  if(help==1){
   Print("SEE README/FILE FOR INSTRUCTIONS. STAY ON MAP");
  }
   //  Make scene visible
   glFlush();
   glutSwapBuffers();
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int xx,int yy)
{
   float fraction =12.0f;
   double z0 = (zmin+zmax)/2;
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
  if (ch == 'r') { // Rain
    fall = RAIN;
    init();
    glutPostRedisplay();
  }
  /*if (ch == 'h') { // Hail
    fall = HAIL;
    init();
    glutPostRedisplay();
  }*/
  if (ch == 'H') { // help
    help=0;
  }
  if (ch == 'h') { // help
    help=1;
  }

  if (ch == 's') { // Snow
    fall = SNOW;
    init();
    glutPostRedisplay();
  }


   if (ch == 'x') { // no poly
    minGrass=32;
    maxGrass=32;
    init();
    glutPostRedisplay();
  }

  if (ch == 'a') { // low poly
    minGrass=29;
    maxGrass=36;
    init();
    glutPostRedisplay();
  }
  if (ch == 'd') { // d medium poly
    minGrass=24;
    maxGrass=44;
    init();
    glutPostRedisplay();
  }
  if (ch == 'g') { // High Poly
    minGrass=0;
    maxGrass=64;
    init();
    glutPostRedisplay();
  }

  if (ch == 'P') { // P More particles
    slowdownSnow=1.0;
    moreParticles=1;
    init();
    glutPostRedisplay();
  }
  if(ch == 'p') { // p Less Particles
    slowdownSnow=2.0;
    moreParticles=0;
    init();
    glutPostRedisplay();
  }

  if(ch == 'y') { // fog density lower
    density=.007;
    init();
    glutPostRedisplay();
  }
  if(ch == 'Y') { // fog density lower still
    density=.003;
    init();
    glutPostRedisplay();
  }

  if(ch == 't') { // fog density back to normal
    density=.01;
    init();
    glutPostRedisplay();
  }

  if (ch == ',') { // really '<' slow down
    if (slowdownSnow < 100.0) slowdownSnow += 0.1;
  }
  if (ch == '.') { // really '>' speed up
    if (slowdownSnow > 0.2) slowdownSnow -= 0.1;
  }

   else if (ch == '0')
      th = ph = 0;

   else if (ch == 'w'){
      x += lx * fraction;
      z += lz * fraction;
      int i=(int) (x+512)/16;
      int j=(int) (y+512)/16;
      y = zs[i][j]-z0+10;
      zoom += 1.0;
   }
   else if (ch == 'W' && dim>1){
      x -= lx * fraction;
      z -= lz * fraction;
      int i=(int) (x+512)/16;
      int j=(int) (y+512)/16;
      y = zs[i][j]-z0+10;
      zoom -= 1.0;
   }

   //  Change field of view angle
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}




/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int xx,int yy)
{
  if(mode==0 || mode==1){
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;

   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
     }
  if(mode==2){

    switch (key) {
      case GLUT_KEY_LEFT :
      angle -= 0.1f;
      lx = sin(angle);
      lz = -cos(angle);
      pan -= 5.0;
      break;
      case GLUT_KEY_RIGHT :
      angle += 0.1f;
      lx = sin(angle);
      lz = -cos(angle);
      pan += 5.0;
      break;
      case GLUT_KEY_UP :
      ly +=.07;
      tilt -= 10.0;
      break;
      case GLUT_KEY_DOWN :
      ly -=.07;
      tilt += 10.0;
      //y+=.02; ANSWER FOR Z HEIGHT


      //x -= lx * fraction;
      //z -= lz * fraction;
      break;
    }
     glutPostRedisplay();
  }
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}


void idle ( ) {
  glutPostRedisplay();
}


int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);

   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   glutInitWindowSize(500,500);
   //  Create window
   glutCreateWindow("Coordinates");
  ReadDEM("saddleback.dem");

  init ();
   //glEnable(GL_DEPTH_TEST);
   //  Register function used to display scene
   glutDisplayFunc(display);


   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);

   //  Pass control to GLUT so it can interact with the user

   texture[0] = LoadTexBMP("bark.bmp");
   texture[1] = LoadTexBMP("bark.bmp");
   texture[2] = LoadTexBMP("grassTex.bmp");
   texture[3] = LoadTexBMP("snowGrassTex.bmp");
   texture[4] = LoadTexBMP("dirt.bmp");
   texture[5] = LoadTexBMP("snow.bmp");

   glutIdleFunc(idle);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}





/*
*  Resources:
*
*  http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/
*
*  OBJECT LOADER glm.c glm.h
*
*  http://www.swiftless.com/tutorials/opengl/camera2.html
*
*  http://www.swiftless.com/tutorials/opengl/fog.html
*
*  http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
*
*  RAIN SIMULATION https://gist.github.com/thaenor/4d9531cc9a7d1c34b998
*
*  Taken work from ex17 ex7 from class.
*
*  All of the obj's were free models, usually from turbosquid.
*
*
*/





















