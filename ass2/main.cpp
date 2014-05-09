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

using namespace std;

vector<int> map;
vector<GLfloat> scene;
vector<GLfloat> material;
vector<GLfloat> house;
vector<GLfloat> car;
vector<GLfloat> sun;

GLfloat MAX_HEIGHT;
GLfloat CAMAERA_HEIGHT;
int width,legnth;

GLfloat rotate_y = 0;
GLfloat rotate_x = 0;
GLfloat rotate_z = 0;

GLfloat zoom = 1;

GLfloat angle = 0;
GLfloat light = 0;

GLfloat light_ambient[4]; 
GLfloat light_diffuse[4];
GLfloat light_specular[4];
GLfloat light_position[] =  {22, 0, 0, 1.0}; // TODO
GLfloat light_direction[]=  {0,0,0};

Vector3f a;
Vector3f b;
Vector3f c;
Vector3f d;

    
GLfloat textures[20];

void initLight(void)
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);

    // Light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);

    glEnable(GL_LIGHT0);

    GLfloat mat_a[] = {material[0], material[1], material[2], 1.0}; 
    GLfloat mat_d[] = {material[3], material[4], material[5], 1.0}; 
    GLfloat mat_s[] = {material[6], material[7], material[8], 1.0}; 
    GLfloat low_sh[]= {5.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat_a); 
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat_d); 
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_s); 
    glMaterialfv(GL_FRONT, GL_SHININESS,low_sh);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); // maintain normal vectors size
}

void init()
{
	glClearColor(1, 1, 1, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix
        
        MAX_HEIGHT = scene.at(2);
        CAMAERA_HEIGHT = scene.at(3);

        light_ambient[0] = scene[4];
        light_ambient[1] = scene[5];
        light_ambient[2] = scene[6];
        light_ambient[3] = 1.0f;
        
        light_diffuse[0] = sun[3];
        light_diffuse[1] = sun[4];
        light_diffuse[2] = sun[5];
        light_diffuse[3] = 1.0f;
        
        light_specular[0] = sun[3];
        light_specular[1] = sun[4];
        light_specular[2] = sun[5];
        light_specular[3] = 1.0f;

        gluPerspective(60, 1, 1, 100);
        gluLookAt(0,0, CAMAERA_HEIGHT, 0,0,0, 0, 1, 0);  //define view direction

        /*initLight();*/

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void printInitVectors(void)
{
    int i = 0;
        cout << " - SCENE - " << endl;
    for(  ; i < (scene).size(); i++) {
        cout << (scene).at(i) << "< " << endl;
    }
        cout << " - MATERIAL- " << endl;
    for(i=0  ; i < (material).size(); i++) {
        cout << (material).at(i) << "< " << endl;
    }
        cout << " - HOUSE - " << endl;
    for(i=0  ; i < (house).size(); i++) {
        cout << (house).at(i) << "< " << endl;
    }
        cout << " - CAR - " << endl;
    for(i=0  ; i < (car).size(); i++) {
        cout << (car).at(i) << "< " << endl;
    }
        cout << " - SUN - " << endl;
    for(i=0  ; i < (sun).size(); i++) {
        cout << (sun).at(i) << "< " << endl;
    }
}

void specialKeys(int key,int x,int y)
{
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
    unsigned int widthBmp, height;
    unsigned int imageSize;   // = widthBmp*height*3

    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file) { 
        printf("Image could not be opened\n"); 
        cout << imagepath << endl;
        return 0; }

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
    widthBmp      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize==0)    imageSize=widthBmp*height*3; // 3 : one byte for each Red, Green and Blue component
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
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, widthBmp, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    free(data);

    return textureID;
}

void loadTextures()
{
    textures[0] = loadBMP_custom("./textures/brick.bmp");
    textures[1] = loadBMP_custom("./textures/roof.bmp");
    textures[2] = loadBMP_custom("./textures/car.bmp");
    textures[3] = loadBMP_custom("./textures/wheel.bmp");
    textures[4] = loadBMP_custom("./textures/grass.bmp");
    textures[5] = loadBMP_custom("./textures/snow.bmp");
    textures[6] = loadBMP_custom("./textures/water.bmp");
    textures[7] = loadBMP_custom("./textures/asphalt.bmp");
    textures[8] = loadBMP_custom("./textures/rock.bmp");
}

void draw_axes(void)
{
    float ORG[3] = {0,0,0};

    float XP[3] = {10,0,0}, XN[3] = {-10,0,0},
          YP[3] = {0,10,0}, YN[3] = {0,-10,0},
          ZP[3] = {0,0,10}, ZN[3] = {0,0,-10};

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

void drawPyramid(GLfloat x1, GLfloat y1, GLfloat z1, 
                 GLfloat x2, GLfloat y2, GLfloat z2, 
                 GLfloat centerX, GLfloat centerY, GLfloat centerZ )
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

void drawHouse(GLfloat x1, GLfloat y1, GLfloat z1, 
               GLfloat x2, GLfloat y2, GLfloat z2)
{
    GLfloat normalizedZ1 = (z1 * MAX_HEIGHT)/255;
    GLfloat normalizedZ2 = (z2 * MAX_HEIGHT)/255;

    glBindTexture (GL_TEXTURE_2D, textures[0]);
    glEnable (GL_TEXTURE_2D);
    drawCube(x1, y1, normalizedZ1, x2, y2, normalizedZ2, 3.5);
    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    GLfloat centerX = x1 + ((x2 - (x1)) / 2);
    GLfloat centerY = y1 + ((y2 - (y1)) / 2);
    GLfloat centerZ = 5;

    // ROOF
    glBindTexture (GL_TEXTURE_2D, textures[1]);
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
    z   = 0.0f;
    y   = 0.0f;
    Cz = Cz*MAX_HEIGHT/255.0f + 0.5;
    GLfloat rCz = Cz;

    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.9,0.9,0.9);
        Cx += 0.3;

        glNormal3f(Cx,Cy,Cz);
        glVertex3f(Cx,Cy,Cz);
        for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
            z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
            y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
            glNormal3f(Cx,y,z);
            glVertex3f(Cx,y,z);
            fPlate[i][0] = Cx;
            fPlate[i][1] = y;
            fPlate[i][2] = z;
        }
        z = rCz*(GLfloat)sin(0.0f) + Cz;
        y = rCz*(GLfloat)cos(0.0f) + Cy;
        glNormal3f(Cx,y,z);
        glVertex3f(Cx,y,z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.9,0.9,0.9);
    Cx -= 0.3;

    glVertex3f(Cx,Cy,Cz);
    for (i=0,angle = 0.0f ; angle < 360 ; angle += 10.0f,i++) {
        z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glNormal3f(Cx,y,z);
        glVertex3f(Cx, y , z);
        bPlate[i][0] = Cx;
        bPlate[i][1] = y;
        bPlate[i][2] = z;
    }
    z = rCz*(GLfloat)sin(0.0f) + Cz;
    y = rCz*(GLfloat)cos(0.0f) + Cy;
    glNormal3f(Cx,y,z);
    glVertex3f(Cx, y , z);
    glEnd();

    // cover wheel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0,0,1);
    for (i = 0 ; i < 36 ; i++) {
        glNormal3f( fPlate[i][0], fPlate[i][1], fPlate[i][2]); 
        glVertex3f( fPlate[i][0], fPlate[i][1], fPlate[i][2]); 
        glNormal3f( bPlate[i][0], bPlate[i][1], bPlate[i][2]); 
        glVertex3f( bPlate[i][0], bPlate[i][1], bPlate[i][2]); 
    }
        glNormal3f( fPlate[0][0], fPlate[0][1], fPlate[0][2]); 
        glVertex3f( fPlate[0][0], fPlate[0][1], fPlate[0][2]); 
        glNormal3f( bPlate[0][0], bPlate[0][1], bPlate[0][2]);
        glVertex3f( bPlate[0][0], bPlate[0][1], bPlate[0][2]);
    glEnd();
}

void drawCarBody(GLfloat x1, GLfloat y1, GLfloat z1, 
                 GLfloat x2, GLfloat y2, GLfloat z2)
{
    z1 = z1*MAX_HEIGHT/255 +0.5;
    z2 = z2*MAX_HEIGHT/255 +0.5;

    drawCube(x1+0.2 ,y1-1.2,z1, 
             x2+0.2 ,y2+1 ,z1,
             1.3);
    drawCube(x1+0.2f,y1   ,1.3, 
             x2+0.2 ,y2-1.3,1.3,
             2.5);
    
    // back winshield
    glColor3f( 1.0,  0.0, 0.0 );
    glBegin(GL_QUADS);
        glNormal3f(x2+0.2,y1,z1);
        glVertex3f( x2+0.2, y1, z1 );
        glNormal3f(x1+0.2,y1,z1);
        glVertex3f( x1+0.2, y1, z1 );
        glNormal3f(x1+0.2,y1,2.5);
        glVertex3f( x1+0.2, y1, 2.5);
        glNormal3f(x2+0.2,y1,2.5);
        glVertex3f( x2+0.2, y1, 2.5 );
    glEnd();

    // front winshield
    glColor3f( 1.0,  0.0, 1.0 );
    glBegin(GL_QUADS);
        glNormal3f(x2+0.2   ,y2-1.3 ,z1);
        glVertex3f(x2+0.2   ,y2-1.3 ,z1);

        glNormal3f(x2+0.2   ,y2-1.3 ,z1);
        glVertex3f(x1+0.2   ,y2-1.3 ,z1);

        glNormal3f(x2+0.2   ,y2-1.3 ,2.5);
        glVertex3f(x1+0.2   ,y2-1.3 ,2.5);

        glNormal3f(x2+0.2   ,y2-1.3 ,2.5);
        glVertex3f(x2+0.2   ,y2-1.3 ,2.5);
    glEnd();
}

void drawCar(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
    glBindTexture (GL_TEXTURE_2D, textures[2]);
    glEnable (GL_TEXTURE_2D);
        draw_wheel(x1   ,y1     ,z1);
        draw_wheel(x1   ,y2     ,z1);
        draw_wheel(x2   ,y1     ,z1);
        draw_wheel(x2   ,y2     ,z2);
    glDisable (GL_TEXTURE_2D); /* disable texture mapping */

    drawCarBody(x1,y1,z1,x2,y2,z2);
}

void draw_plane(void)
{
    glBegin(GL_LINE_LOOP);
        glColor3f(0,0,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,MAX_HEIGHT);
        glVertex3f(sun[0],sun[1],sun[2]);
    glEnd();
}

void drawSun(void)
{
    Vector3f a;
    Vector3f b;
    Vector3f c;
    
    glPushMatrix();
        a.x = 0;
        a.y = 0;
        a.z = MAX_HEIGHT;
        b.x = sun[0];
        b.y = sun[1];
        b.z = sun[2];
        c = Vector3f::crossProduct(a,b);

        glRotatef(angle,c.x,c.y,c.z);
        glTranslatef(sun[0],sun[1],sun[2]);
        
        glColor3f(sun[3],sun[4],sun[5]);
        glutSolidSphere(1,36,36);
        
        light_direction[0] = -sun[0];
        light_direction[1] = -sun[1];
        light_direction[2] = -sun[2];

        light_position[0] = sun[0];
        light_position[1] = sun[1];
        light_position[2] = sun[2];

        /*initLight();*/

    glPopMatrix();
}

void rotateSun(void) 
{
    angle += 0.6; // 10 seconds loop 
    glutPostRedisplay();
}

void resetlight(void) 
{
    initLight();
}
int normalVectorsTest(GLfloat x[], GLfloat y[], GLfloat z[],GLfloat realZ[])
{
    Vector3f vectorAD,vectorAC,vectorBA,vectorBC;
    Vector3f crossProductP1,crossProductP2;

    a = Vector3f(x[0],y[0],realZ[0]);
    b = Vector3f(x[1],y[1],realZ[1]);
    c = Vector3f(x[2],y[2],realZ[2]);
    d = Vector3f(x[3],y[3],realZ[3]);

    a.normalize();
    b.normalize();
    c.normalize();
    d.normalize();
    /*cout << "a.x = " << a.x << " ";
    cout << "a.y = " << a.y << " ";
    cout << "a.z = " << a.z << " " << endl;*/

    vectorAD.fromTo(a,d); // create vectors from a to d
    vectorAC.fromTo(a,c);// create vectores from a to c
    vectorBA.fromTo(b,a); // create vectors from b to a
    vectorBC.fromTo(b,c);// create vectores from b to c*/

    crossProductP1 = Vector3f::crossProduct(vectorAD,vectorAC);
    crossProductP2 = Vector3f::crossProduct(vectorBA,vectorBC);
    
    /*cout << "crossProductP1.x = " << crossProductP1.x << " " << endl;
    cout << "crossProductP2.x = " << crossProductP2.x << " " << endl;
    cout << "crossProductP1.y = " << crossProductP1.y << " " << endl;
    cout << "crossProductP2.y = " << crossProductP2.y << " " << endl;
    cout << "crossProductP1.z = " << crossProductP1.z << " " << endl;
    cout << "crossProductP2.z = " << crossProductP2.z << " " << endl;
    cout << " _________________________________________________" << endl;*/

    /*cout << crossProductP1 << endl;*/

    if (crossProductP1.x == crossProductP2.x &&
        crossProductP1.y == crossProductP2.y &&
        crossProductP1.z == crossProductP2.z) 
        return true;

    return false;
}

void drawQuad(GLfloat x[], GLfloat y[], GLfloat z[],GLfloat realZ[],
              GLfloat tX[], GLfloat tY[], GLfloat textureId)
{

    bool result = normalVectorsTest(x,y,z,realZ);
    Vector3f planeAC = Vector3f::crossProduct(a,c);
    Vector3f planeBD = Vector3f::crossProduct(b,d);

    glColor3f( 1.0,  1.0, 1.0 );
    glBindTexture (GL_TEXTURE_2D, textureId);
    glEnable (GL_TEXTURE_2D);
	glBegin(GL_QUADS);

    switch(result) {
        case true:
            glNormal3f(x[0],y[0],z[0]);
            glTexCoord2f (tX[0],tY[0]); /* upper left corner of image */
            glVertex3f(x[0], y[0], z[0]);

            glNormal3f(x[1],y[1],z[1]);
            glTexCoord2f (tX[2],tY[1]); /* upper right corner of image */
            glVertex3f(x[1], y[1], z[1]);

            glNormal3f(x[2],y[2],z[2]);
            glTexCoord2f (tX[2],tY[2]); /* lower right corner of image */
            glVertex3f(x[2], y[2], z[2]);

            glNormal3f(x[3],y[3],z[3]);
            glTexCoord2f (tX[3],tY[3]); /* lower left corner of image */
            glVertex3f(x[3], y[3], z[3]);
            break;
        
        case false:
            Vector3f two    = Vector3f(x[1],y[1],z[1]);
            Vector3f four   = Vector3f(x[3],y[3],z[3]);
            Vector3f first  = Vector3f(x[0],y[0],z[0]);
            Vector3f third  = Vector3f(x[2],y[2],z[2]);

            Vector3f vectorTwoFirst,vectorTwoThird;
            Vector3f vectorFourFirst,vectorFourThird;

            vectorTwoFirst.fromTo(two,first);
            vectorTwoThird.fromTo(two,third);
            vectorFourFirst.fromTo(four,first);
            vectorFourThird.fromTo(four,third);

            Vector3f crossProductForTwo = Vector3f::crossProduct
                                          (vectorTwoFirst,vectorTwoThird);
            Vector3f crossProductForFour = Vector3f::crossProduct
                                          (vectorFourFirst,vectorFourThird);
            
            
            glNormal3f(crossProductForTwo.x + crossProductForFour.x,
                       crossProductForTwo.y + crossProductForFour.y,
                       crossProductForTwo.z + crossProductForFour.z);
            glTexCoord2f (tX[0],tY[0]); /* upper left corner of image */
            glVertex3f(x[0], y[0], z[0]);

            glNormal3f(crossProductForTwo.x,crossProductForTwo.y,crossProductForTwo.z);
            glTexCoord2f (tX[2],tY[1]); /* upper right corner of image */
            glVertex3f(x[1], y[1], z[1]);

            glNormal3f(crossProductForTwo.x + crossProductForFour.x,
                       crossProductForTwo.y + crossProductForFour.y,
                       crossProductForTwo.z + crossProductForFour.z);
            glTexCoord2f (tX[2],tY[2]); /* lower right corner of image */
            glVertex3f(x[2], y[2], z[2]);

            glNormal3f(crossProductForFour.x,crossProductForFour.y,crossProductForFour.z);
            glTexCoord2f (tX[3],tY[3]); /* lower left corner of image */
            glVertex3f(x[3], y[3], z[3]);
            break;
    }
	glEnd();

	glDisable (GL_TEXTURE_2D); /* disable texture mapping */
}

GLfloat chooseTexturesFromMap(GLfloat x[],GLfloat y[],GLfloat z[])
{
    if (z[0] == z[1] && z[2] == z[0] && z[3] == z[0]) {

        if (fmod(255.0f*z[0]/MAX_HEIGHT, 2.0) == 0)
            return textures[6]; //water
        else
            return textures[7]; //asphalt

    } else {
        GLfloat range;
        if( z[0] == z[1] )
        {
            range = 255.0f * z[0]/MAX_HEIGHT;
        } else {
            if( z[0] == z[2])
                range = 255.0f * z[0]/MAX_HEIGHT;
            else
                range = 255.0f * z[1]/MAX_HEIGHT;
        }

        if (range < 100) {
            return textures[4]; // grass
        } else if (range <= 200 && range >= 100) {
            return textures[8]; // rocks
        } else if (range > 200) {
            return textures[5]; // snow
        }
    }
}

void buildPolygons() 
{
    int i = 0;          // first cell first row
    int j = width + 1;  // first cell second row
    GLfloat dw = 0;
    GLfloat dl = 0;
    GLfloat distanceW = scene[0]/width;
    GLfloat distanceL = scene[1]/legnth;

    GLfloat x[4] = {0,0,0,0};
    GLfloat y[4] = {0,0,0,0};
    GLfloat z[4] = {0,0,0,0};
    GLfloat realZ[4] = {0,0,0,0};
    GLfloat tX[4] = {0,1,1,0};
    GLfloat tY[4] = {1,1,0,0};

    // Loop over vector, place i index first cell, j index in first cell second line.
    for( int iter = 1 ; j <= (int)map.size()-2 ; i++ , j++, dw++, iter++)
    {
        // every time we end line
        if (iter == width+1){
            i++;
            j++;
            dl++;
            dw=0;
            iter = 1;
        }

        y[0] = (dl * distanceL -scene[0]/2.0f);
        x[0] = (dw * distanceW -scene[0]/2.0f);
        z[0] = ((MAX_HEIGHT*map[i])/255.0f);
        realZ[0] = map[i];

        y[1] = (dl * distanceL -scene[0]/2.0f);
        x[1] = ((dw+1) * distanceW -scene[0]/2.0f);
        z[1] = ((MAX_HEIGHT*map[i+1])/255.0f);
        realZ[1] = map[i+1];
        
        y[2] = ((dl+1) * distanceL -scene[0]/2.0f);
        x[2] = ((dw+1) * distanceW -scene[0]/2.0f);
        z[2] = ((MAX_HEIGHT*map[j+1])/255.0f);
        realZ[2] = map[j+1];
        
        y[3] = ((dl+1) * distanceL -scene[0]/2.0f);
        x[3] = (dw * distanceW -scene[0]/2.0f);
        z[3] = ((MAX_HEIGHT*map[j])/255.0f);
        realZ[3] = map[j];
            
        GLfloat textureId = chooseTexturesFromMap(x,y,realZ);
        drawQuad(x,y,z,tX,tY,realZ,textureId);
        continue;

        if(z[0]==z[1] && z[0]!=z[2] && z[0]!=z[3]) {
            double t1, t2;
            GLfloat newX[4];
            GLfloat newY[4];
            GLfloat newZ[4];
            GLfloat textureId;
            GLfloat range;

            if(map[i] > 200) {
                range = 200*MAX_HEIGHT/255.0f;
                t1 = ( (range-z[0]) / (z[3]-z[0]) );
                t2 = ( (range-z[0]) / (z[2]-z[1]) );
            } else {
                range = 100*MAX_HEIGHT/255.0f;
                t1 = ( (range-z[0]) / (z[3]-z[0]) );
                t2 = ( (range-z[0]) / (z[2]-z[1]) );
            }

            newX[0] = x[0];
            newY[0] = y[0];
            newZ[0] = z[0];

            newX[1] = x[1];
            newY[1] = y[1];
            newZ[1] = z[1];

            newX[2] = x[1] + t2*(x[2]-x[1]);
            newY[2] = y[1] + t2*(y[2]-y[1]);
            newZ[2] = range;

            newX[3] = x[0] + t1*(x[3]-x[0]);
            newY[3] = y[0] + t1*(y[3]-y[0]);
            newZ[3] = range;

            textureId = chooseTexturesFromMap(newX,newY,newZ);
            drawQuad(newX,newY,newZ,realZ,tX,tY,textureId);

            newX[0] = newX[3];
            newY[0] = newY[3];
            newZ[0] = newZ[3];

            newX[1] = newX[2];
            newY[1] = newY[2];
            newZ[1] = newZ[2];

            newX[2] = x[2];
            newY[2] = y[2];
            newZ[2] = z[2];

            newX[3] = x[3];
            newY[3] = y[3];
            newZ[3] = z[3];

            textureId = chooseTexturesFromMap(newX,newY,newZ);
            drawQuad(newX,newY,newZ,realZ,tX,tY,textureId);

        } else {
            GLfloat textureId = chooseTexturesFromMap(x,y,z);
            drawQuad(x,y,z,tX,tY,realZ,textureId);
        }
    }
}

void parseMap(vector<int> *map, int* width, int* legnth)
{
    int i = 0;

    ifstream file ( "./input/map.csv" );

    while (file.good()) {
        string s;

        if (!getline(file,s)) break;
        istringstream ss(s);

        while(ss) {
            string s;

            if (!getline(ss,s,',')) break;
            istringstream is(s);

/*<<<<<<< HEAD
            if (i == 0)         { *width = stoi(s);  i++; } 
            else { if (i == 1)  { *legnth = stoi(s); i++;} 
                else            {(map)->push_back(stoi(s));
                }
=======*/
            if (i == 0)         { is >> *width;  i++; }
            else { if (i == 1)  { is >>  *legnth; i++; }
                   else         { int num; is >> num; (map)->push_back(num); }
            }
        }
    }
    file.close();
}

void parseInit(vector<GLfloat> *material, vector<GLfloat> *house, 
        vector<GLfloat> *car, vector<GLfloat> *sun)
{
    int pos;
    int iter = 0;

    ifstream file ( "./input/init.txt" );

    while (file.good()) {
        string s;

        if (!getline(file,s)) break;
        istringstream ss(s);

        while(ss) {
            string s;

            if (!getline(ss,s,',')) break;

            if (s[0] > 'a' && s[0] < 'z')
                iter++;

            pos = s.find(" ");         // position of "live" in str

            if (pos != -1)
                s = s.substr(pos + 1);

            switch(iter) {

                case 1: 
                    (scene).push_back(atof(s.c_str()));
                    break;
                case 2: 
                    (material)->push_back(atof(s.c_str()));
                    break;
                case 3:
                    (house)->push_back(atof(s.c_str()));
                    break;
                case 4:
                    (car)->push_back(atof(s.c_str()));
                    break;
                case 5:
                    (sun)->push_back(atof(s.c_str()));
                    break;
            }
        }
    }
    file.close();
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

void mydisplay(void)
{	
    // always make sure to be on MODEL_VIEW matric here!
    glLoadIdentity();

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    draw_axes();

    drawCar(car[0],car[1],car[2],car[3],car[4],car[5]);
    drawHouse(house[0],house[1],house[2],
             house[3],house[4],house[5]);
    drawSun();
    buildPolygons();

    glutSwapBuffers();
}

int main(int argc, char**argv) {
    glutInit(&argc, argv);

    // Parsing input files
    parseMap(&map,&width,&legnth);
    parseInit(&material,&house, &car, &sun);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
    
    init();
    loadTextures();

    // Callback functions
	glutDisplayFunc(mydisplay);
	glutIdleFunc(rotateSun);
    glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
