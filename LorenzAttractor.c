#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//Standard C libs ^^

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//Helps OS compatilbility ^^

//Global variables:
int th = 0;
int ph = 0;
double sc = 1;
double dim = 100; //

//Lorenz params
double s  = 10;
double b  = 2.6666;
double r  = 28;

//PRINT TAKEN FROM ex6
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

void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //clears previous image
  glLoadIdentity(); //loads identity matrix to clear previous transformations
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0); //Rotate functions for x and y axis - taken from ex6 since I want to view it in the same intuitive way

  glPointSize(5);
  glBegin(GL_LINE_STRIP); //GL_LINE_STRIP so its a continuous line as we add vertices. GL_LINES makes it broken up


  //BEGIN LORENZ.C CODE, modified so it displays as a graphic and not as lines of text
  int i;
  /*  Coordinates  */
  double x = 1;
  double y = 1;
  double z = 1;
  /*  Time step  */
  double dt = 0.001;

  //Moved these declarations outside of the loop for efficiency
  double dx = 0;
  double dy = 0;
  double dz = 0;

  //for some color fun
  double rx = 0;
  double gy = 0;
  double bz = 0;
  /*
   *  Integrate 50,000 steps (50 time units with dt = 0.001)
   *  Explicit Euler integration
   */

  for (i=0;i<50000;i++)
  {
     dx = s*(y-x);
     dy = x*(r-z)-y;
     dz = x*y - b*z;
     x += dt*dx;
     y += dt*dy;
     z += dt*dz;
     (x>=0) ? (rx = x/60.0) : (rx = -1*x/60); //The color is now a function of the x, y, z coordinates
     (y>=0) ? (gy = y/60.0) : (gy = -1*y/60); //and normalized so theyre less than 1 and abs value so they can't be negative
     (z>=0) ? (bz = z/60.0) : (bz = -1*z/60);
     //(rx>=0.5) ? (rx=rx) : (rx+=0.3); //To make it more red
     glColor3f(rx,gy,bz); //We could make the color a function of many things for many cool effects.
     glVertex3f(x*sc,y*sc,z*sc);
  }
  glEnd();
  //END LORENZ.C CODE

  //Draw X, Y, Z Axes
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(90,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,90,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,90);
  glEnd();
  //Above and below blocks are borrowed from ex6 and adjusted to fit the scale of the attractor
  glRasterPos3d(90,0,0);
  Print("X");
  glRasterPos3d(0,90,0);
  Print("Y");
  glRasterPos3d(0,0,90);
  Print("Z");


  glFlush();
  glutSwapBuffers();
}

void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0'){
      th = ph = 0;
      sc = 1; //reset scale too
      s = 10;
      b = 2.666;
      r = 28; }
   else if (ch == '+') //scale it up
      sc+=0.1;
   else if (ch == '-') //scale it down
      sc-=0.1;
   else if (ch == 83) //115 = S
      s+=0.5;
   else if (ch == 115) //115 = s
      s-=0.5;
   else if (ch == 66) //66 = B
      b+=0.1;
   else if (ch == 98) //98 = b
      b-=0.1;
   else if (ch == 82) //82 = R
      r+=1;
   else if (ch == 114) //114 = r
      r-=1;
   //So we can increase each parameter of the lorenz attractor s,b,r
   //with their capitals, and scale down with the lower case of each
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void reshape(int width,int height) // !!! BORROWED FROM ex6 !!!
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

int main(int argc, char* argv[])
{
  glutInit(&argc,argv); //Initialize glut with user input
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Initialize RGB, double buffering, and depth testing
  glutInitWindowSize(500,500); //Initialize window size
  glutCreateWindow("Homework 2 - Zach Morrissey - Lorenz Attractor"); //Create window with input string as title
  glEnable(GL_DEPTH_TEST);//allow depth testing
  glutDisplayFunc(display); //calls the display function which draws the program
  glutReshapeFunc(reshape);
  glutSpecialFunc(special); //calls a function for arrow key interaction
  glutKeyboardFunc(key); //calls a function for keyboard interaction
  glutMainLoop();

  return 0;
}
