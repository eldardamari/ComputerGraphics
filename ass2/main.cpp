#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "./Vector3f.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else 
    #include <GL/glut.h>
#endif

#define max_hight 5.5
using namespace std;

GLfloat rotate_y = 0;
GLfloat rotate_x = 0;
GLfloat rotate_z = 0;
GLfloat zoom = 1;
GLfloat MAX_HEIGHT = 5.5;

GLfloat angle = 0;

GLfloat textures[20];

void init()
{
	glClearColor(1, 1, 1, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

	gluPerspective(60, 1, 1, 100);
	gluLookAt(0,0, 30, 0,0,0, 0, 1, 0);  //define view direction

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void specialKeys(int key,int x,int y) {
    glMatrixMode(GL_PROJECTION);

    switch(key) {
        case GLUT_KEY_RIGHT:
            glRotatef( -4, 0.0, 0.0, 1.0 );
            break;
        case GLUT_KEY_LEFT:
            glRotatef( 4, 0.0, 0.0, 1.0 );
            break;
        case GLUT_KEY_UP:
            glRotatef( 4, 1.0, 0.0, 0.0 );
            break;
        case GLUT_KEY_DOWN:
            glRotatef( -4, 1.0, 0.0, 0.0 );
            break;
        case GLUT_KEY_F2:
            zoom = 1;
            zoom += 0.1;
            glScalef(zoom,zoom,zoom);
            break;
        case GLUT_KEY_F3:
            zoom = 1;
            zoom -= 0.1;
            glScalef(zoom,zoom,zoom);
            break;
    }

    glMatrixMode(GL_MODELVIEW);

    //  Request display update
    glutPostRedisplay();
}

GLuint loadBMP_custom(const char * imagepath)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file) { printf("Image could not be opened\n"); return 0; }

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    free(data);

    return textureID;
}

void loadTextures()
{
    textures[1] = loadBMP_custom("brick.bmp");
    textures[2] = loadBMP_custom("roof.bmp");
    textures[3] = loadBMP_custom("car.bmp");
    textures[4] = loadBMP_custom("wheel.bmp");
}

void draw_axes(void)
{
    float ORG[3] = {0,0,0};

    float XP[3] = {15,0,0}, XN[3] = {-15,0,0},
          YP[3] = {0,15,0}, YN[3] = {0,-15,0},
          ZP[3] = {0,0,15}, ZN[3] = {0,0,-15};

    glLineWidth (2.0);
    glBegin (GL_LINES);

    glColor3f (1,0,0); // X axis is red.
    glVertex3fv (XN);
    glVertex3fv (XP);

    glColor3f (0,1,0); // Y axis is green.
    glVertex3fv (YN);
    glVertex3fv (YP);

    glColor3f (0,0,1); // Z axis is blue.
    glVertex3fv (ZN);
    glVertex3fv (ZP);

    glEnd();
}

void drawCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat h)
{
      glColor3f( 1.0,  1.0, 1.0 );

      //side - FRONT
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x1, y1, z1 );      // P1 is red
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x1, y1, h  );      // P2 is green
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x2, y1, h  );      // P3 is blue
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x2, y1, z1 );      // P4 is purple

      glEnd();

      //side - BACK
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x1, y2, z2 );
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x1, y2, h );
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x2, y2, h );
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - RIGHT
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x2, y1, z1 );
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x2, y1, h );
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x2, y2, h );
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - LEFT
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x1, y1, z1 );
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x1, y1, h );
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x1, y2, h );
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x1, y2, z2 );
      glEnd();

      //side - TOP
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x1, y1, h );
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x1, y2, h );
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x2, y2, h );
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x2, y1, h );
      glEnd();

      //side - BOTTOM
      glBegin(GL_QUADS);
      glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
      glVertex3f( x1, y1, z1 );
      glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
      glVertex3f( x1, y2, z1 );
      glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
      glVertex3f( x2, y2, z2 );
      glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
      glVertex3f( x2, y1, z2 );
      glEnd();
}

void drawPyramid(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat centerX, GLfloat centerY, GLfloat centerZ )
{
    glColor3f (   1.0,    0.0,    0.0 );

    // ROOF - Right
    glBegin(GL_TRIANGLES);
    glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
    glVertex3f( x2, y1, z1 );
    glTexCoord2f (0.5f, 1.0f); /* upper point corner of image */
    glVertex3f( centerX, centerY, centerZ );
    glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
    glVertex3f( x2, y2, z2 );
    glEnd();

    // ROOF - Back
    glBegin(GL_TRIANGLES);
    glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
    glVertex3f(   x1,    y2,   z2   );
    glTexCoord2f (0.5f, 1.0f); /* upper point corner of image */
    glVertex3f( centerX, centerY, centerZ );
    glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
    glVertex3f(     x2,    y2,   z1   );
    glEnd();

    // ROOF - Left
    glBegin(GL_TRIANGLES);
    glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
    glVertex3f( x1, y1, z1 );
    glTexCoord2f (0.5f, 1.0f); /* upper point corner of image */
    glVertex3f( centerX, centerY, centerZ );
    glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
    glVertex3f( x1, y2, z1 );
    glEnd();

    // ROOF - Front
    glBegin(GL_TRIANGLES);
    glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
    glVertex3f( x1, y1, z1 );
    glTexCoord2f (0.5f, 1.0f); /* upper point corner of image */
    glVertex3f( centerX, centerY, centerZ );
    glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
    glVertex3f( x2, y1, z2 );
    glEnd();
}

void drawHouse(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{

    GLfloat normalizedZ1 = (z1 * MAX_HEIGHT)/255;
    GLfloat normalizedZ2 = (z2 * MAX_HEIGHT)/255;

    glBindTexture (GL_TEXTURE_2D, textures[1]);
    glEnable (GL_TEXTURE_2D);
    drawCube(x1, y1, normalizedZ1, x2, y2, normalizedZ2, 3.5);
    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    GLfloat centerX = x1 + ((x2 - (x1)) / 2);
    GLfloat centerY = y1 + ((y2 - (y1)) / 2);
    GLfloat centerZ = 5;

    // ROOF
    glBindTexture (GL_TEXTURE_2D, textures[2]);
    glEnable (GL_TEXTURE_2D);
    drawPyramid(x1, -2.5, 3.5, x2, -5.5, 3.5, centerX, centerY, centerZ );
    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    // DOOR
    glColor3f (   0.5,    0.5,    0.5 );
    glBegin(GL_QUADS);
    glVertex3f( centerX+0.5, y1, normalizedZ1 );
    glVertex3f( centerX+0.5, y1, normalizedZ1 + 1.5 );
    glVertex3f( centerX-0.5, y1, normalizedZ2 + 1.5 );
    glVertex3f( centerX-0.5, y1, normalizedZ2 );
    glEnd();

    // WINDOW - Right
    glColor3f (   0.0,    1.0,    0.0 );
    glBegin(GL_QUADS);
    glVertex3f( x2, centerY+1, normalizedZ1 + 1.5 );
    glVertex3f( x2, centerY+1, normalizedZ1 + 2.5 );
    glVertex3f( x2, centerY-1, normalizedZ2 + 2.5 );
    glVertex3f( x2, centerY-1, normalizedZ2 +1.5 );
    glEnd();

    // WINDOW - Left
    glColor3f (   0.0,    1.0,    0.0 );
    glBegin(GL_QUADS);
    glVertex3f( x1, centerY+1, normalizedZ1 + 1.5 );
    glVertex3f( x1, centerY+1, normalizedZ1 + 2.5 );
    glVertex3f( x1, centerY-1, normalizedZ2 + 2.5 );
    glVertex3f( x1, centerY-1, normalizedZ2 +1.5 );
    glEnd();
}

void draw_wheel(GLfloat Cx, GLfloat Cy, GLfloat Cz)
{
    GLfloat z, y,angle;
    GLfloat fPlate[36][3];
    GLfloat bPlate[36][3];
    GLfloat GL_PI  = 3.1415926535897932384626433832795;
    int i;
    z = 0.0f;
    y = 0.0f;
    GLfloat rCz = Cz;
    /*Cz = Cz*max_hight/255.0f;*/

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.9,0.9,0.9);
    Cx += 0.2;

    glVertex3f(Cx,Cy,Cz);
    for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
        z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        fPlate[i][0] = Cx;
        fPlate[i][1] = y;
        fPlate[i][2] = z;
    }
    z = rCz*(GLfloat)sin(0.0f) + Cz;
    y = rCz*(GLfloat)cos(0.0f) + Cy;
    glVertex3f(Cx , y , z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.9,0.9,0.9);
    Cx -= 0.2;

    glVertex3f(Cx,Cy,Cz);
    for (i=0,angle = 0.0f ; angle < 360 ; angle += 10.0f,i++) {
        z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        bPlate[i][0] = Cx;
        bPlate[i][1] = y;
        bPlate[i][2] = z;
    }
    z = rCz*(GLfloat)sin(0.0f) + Cz;
    y = rCz*(GLfloat)cos(0.0f) + Cy;
    glVertex3f(Cx, y , z);
    glEnd();

    // cover wheel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0,0,1);
    for (i = 0 ; i < 36 ; i++) {
        glVertex3f( fPlate[i][0], fPlate[i][1], fPlate[i][2]); 
        glVertex3f( bPlate[i][0], bPlate[i][1], bPlate[i][2]); 
    }
        glVertex3f( fPlate[0][0], fPlate[0][1], fPlate[0][2]); 
        glVertex3f( bPlate[0][0], bPlate[0][1], bPlate[0][2]);
    glEnd();
}

void drawWindShield()
{
    glColor3f( 1.0,  0.0, 0.0 );
    glBegin(GL_QUADS);
        glVertex3f( 1.0, 4.0f, 1.5f );
        glVertex3f( 0.2f, 4.0f, 1.5f );
        glVertex3f( 0.2f, 4.0f, 3.5f);
        glVertex3f( 1.0f,4.0f, 3.5f );
    glEnd();

    glColor3f( 1.0,  0.0, 1.0 );
    glBegin(GL_QUADS);
        glVertex3f( 0.2f, 0.0f, 1.5f );
        glVertex3f( 1.0f, 0.0f, 1.5f );
        glVertex3f( 1.0f, 0.0f, 3.5f);
        glVertex3f( 0.2f,0.0f, 3.5f );
    glEnd();
}

void drawCarBody(void)
{
    glBindTexture (GL_TEXTURE_2D, textures[3]);
    glEnable (GL_TEXTURE_2D);
    drawCube(0.2f,-1.2f,1.0f, 1.0f, 6.2f,1.0f,2.5f);
    drawCube(0.2f,0.0f,3.5f, 1.0f, 4.0f,3.5f,2.0f);
    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    drawWindShield();
}

void drawCar(void) {
    draw_wheel(0.0f ,0.0f   ,1.0f);
    draw_wheel(1.0f ,0.0f   ,1.0f);
    draw_wheel(0.0f ,5.0f  ,1.0f);
    draw_wheel(1.0f ,5.0f  ,1.0f);

    drawCarBody();
}

void drawSun(void)
{
    glPushMatrix();

    Vector3f a;
    a.x = 0;
    a.y = 0;
    a.z = MAX_HEIGHT;
    Vector3f b;
    b.x = 15;
    b.y = 7;
    b.z = 8;
    Vector3f c;
    c = Vector3f::crossProduct(a,b);

        glRotatef(angle,c.x,c.y,c.z);

        glTranslatef(15,7,8);
        glColor3f(0.5f, 0.7f , 0.0f);
        glutSolidSphere(1,36,36);
    glPopMatrix();
}

void rotateSun(void) {
    angle += 1.5;
    glutPostRedisplay();
}

void draw_plane(void) {
    glBegin(GL_LINE_LOOP);

        glColor3f(0,0,0);

        glVertex3f(0,0,0);
        glVertex3f(0,0,MAX_HEIGHT);
        glVertex3f(15,7,8);

    glEnd();

}

void mydisplay(void)
{	
    // always make sure to be on MODEL_VIEW matric here!
    glLoadIdentity();

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    draw_axes();

    drawCar();
    drawHouse(2.5,-2.5,7,5,-5.5,7);
    drawSun();
//    draw_plane();

    glutSwapBuffers();
}

void test(void)
{
    glLoadIdentity ();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    GLuint texture = loadBMP_custom("brick.bmp");
    glBindTexture (GL_TEXTURE_2D, texture);

    glEnable (GL_TEXTURE_2D);
    glBegin (GL_QUADS);
        glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
        glVertex3f (-2.0f, -2.0f, 0.0f);
        glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
        glVertex3f (2.0f, -2.0f, 0.0f);
        glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
        glVertex3f (2.0f, 2.0f, 0.0f);
        glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
        glVertex3f (-2.0f, 2.0f, 0.0f);
    glEnd ();

//    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    glutSwapBuffers();
}

int main(int argc, char**argv) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
    
    init();

    loadTextures();

    // Callback functions
	glutDisplayFunc(mydisplay);
//	glutDisplayFunc(test);
	glutIdleFunc(rotateSun);
    glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
