/*  Sam Skolnekovich HW6
 *  CSCI 4229
 *  Textures and Lighting
 *
 *  Demonstrates using lighting and textures.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[2]; // Texture names
int scene=1;




/*
* Resource for torus: https://au.answers.yahoo.com/question/index?qid=20120807204334AA0DJVa
* normal vector calculations done on my own.
*
*/


void chain(double x,double y,double z,
                 double dx,double dy,double dz,
                 double xa,double ya,double za) {
      glPushMatrix();
      glTranslated(x,y,z);
      glRotated(xa,0,1,0);
      glRotated(ya,0,0,1);
      glRotated(za,1,0,0);
      glScaled(dx,dy,dz);

  //numc=rseg, numt=cseg
      int numc = 100, numt = 100;


  glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);


      double TWOPI = 2 * M_PI;
      for (int i = 0; i < numc; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numt; j++) {
          for (int k = 1; k >= 0; k--) {

            double s = (i + k) % numc + 0.5;
            double t = j % numt;
            //norms calc
            double omega=s * TWOPI / numc;
            double theta=t * TWOPI / numt;

            //verts calc
            double x = (1 + 0.1 * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
            double z = (1 + 0.1 * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
            double y = 0.1 * sin(s * TWOPI / numc);

            //tex calc
            double u = (i + k) / (float) numc;
            double v = t / (float) numt;

            glNormal3d(sin(theta)*cos(omega),sin(omega),cos(theta)*cos(omega));
            glTexCoord2d(u, v);
            glVertex3d(x, y, z);
          }
        }

        glEnd();
        }
  glPopMatrix();
}





void donut(double x,double y,double z,
                 double dx,double dy,double dz,
                 double xa,double ya,double za) {
      glPushMatrix();
      glTranslated(x,y,z);
      glRotated(xa,0,1,0);
      glRotated(ya,0,0,1);
      glRotated(za,1,0,0);
      glScaled(dx,dy,dz);

  //numc=rseg, numt=cseg
      int numc = 100, numt = 100;


  glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);


      double TWOPI = 2 * M_PI;
      for (int i = 0; i < numc; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= numt; j++) {
          for (int k = 1; k >= 0; k--) {

            double s = (i + k) % numc + 0.5;
            double t = j % numt;
            //norms calc
            double omega=s * TWOPI / numc;
            double theta=t * TWOPI / numt;

            //verts calc
            double x = (1 + 0.1 * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
            double z = (1 + 0.1 * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
            double y = 0.1 * sin(s * TWOPI / numc);

            //tex calc
            double u = (i+k)/(float) numc;
            double v = t/ (float) numt;

            glNormal3d(sin(theta)*cos(omega),sin(omega),cos(theta)*cos(omega));
            glTexCoord2d(u, v);
            glVertex3d(x, y, z);
          }
        }

        glEnd();
        }
  glPopMatrix();
}







/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}


void keychain(){
   chain(0,1.7,0,1,1,1,0,90,90);
   chain(0,0,0,1,1,1,0,90,0);
   chain(0,-1.7,0,1,1,1,0,90,90);
   chain(.3,3.4,0,1,1,1,0,90,0);
   chain(.3,-3.4,0,1,1,1,0,90,0);
   chain(1,-4.5,0,1,1,1,0,90,90);
   chain(.6,5.1,0,1,1,1,0,90,90);
   chain(1.7,-5.1,0,1,1,1,0,90,0);
   chain(1.3,6.5,0,1,1,1,0,90,0);
   chain(2.7,-5.7,0,1,1,1,0,90,90);
   chain(2.1,7.4,0,1,1,1,0,90,90);

  glPushMatrix();
   glTranslated(7,0,0);
   glRotated(180,0,1,0);
   glRotated(0,0,0,1);
   glRotated(0,1,0,0);

  chain(0,1.7,0,1,1,1,0,90,90);
   chain(0,0,0,1,1,1,0,90,0);
   chain(0,-1.7,0,1,1,1,0,90,90);
   chain(.3,3.4,0,1,1,1,0,90,0);
   chain(.3,-3.4,0,1,1,1,0,90,0);
   chain(1,-4.5,0,1,1,1,0,90,90);
   chain(.6,5.1,0,1,1,1,0,90,90);
   chain(1.7,-5.1,0,1,1,1,0,90,0);
   chain(1.3,6.5,0,1,1,1,0,90,0);
   chain(2.7,-5.7,0,1,1,1,0,90,90);
   chain(2.1,7.4,0,1,1,1,0,90,90);
   chain(3.3,7.7,0,1,1,1,0,0,0);
   chain(3.3,-5.9,0,1,1,1,0,0,0);
  glPopMatrix();

}



/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=2.0;
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene

   if(scene==0){
   keychain();
    dim=10.0;
  }
  if(scene==1){
    donut(0,0,0,1,1,1,0,0,0);
    dim=4.0;
    ylight=2.0;
  }

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);

   if(scene==0)
   Print("Title: Keychain, Change scene:'c'");
  if(scene==1)
    Print("Title:donut,change scene:'c'");
   if (light)
   {
      glWindowPos2i(5,25);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
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
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;



   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;

     else if (ch == 'c')
      scene += 1;
      if(scene>1)
         scene=0;

   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Textures and Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("donuts.bmp");
   texture[1] = LoadTexBMP("keychain.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
