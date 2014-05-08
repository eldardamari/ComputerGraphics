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
int width,legnth;

GLfloat rotate_y = 0;
GLfloat rotate_x = 0;
GLfloat rotate_z = 0;
GLfloat zoom = 1;
GLfloat angle = 0;
GLfloat light = 0;

GLfloat light_ambient[] =   {0.2, 0.2, 0.2, 1.0}; 
GLfloat light_diffuse[] =   {1.0, 1.0, 1.0, 1.0}; 
GLfloat light_specular[] =  {1.0, 1.0, 1.0, 1.0};
GLfloat light_position[] =  {22, 0, 0, 1.0};
GLfloat light_direction[]=  {0,0,0};

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

        gluPerspective(60, 1, 1, 100);
        gluLookAt(0,0, 30, 0,0,0, 0, 1, 0);  //define view direction

        MAX_HEIGHT = scene.at(2);
        initLight();

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

    return textureID;
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

void drawPyramid(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat centerX, GLfloat centerY, GLfloat centerZ )
{
    glColor3f (   1.0,    0.0,    0.0 );

    // ROOF - Right
    glBegin(GL_TRIANGLES);
    glVertex3f( x2, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x2, y2, z2 );
    glEnd();

    // ROOF - Back
    glBegin(GL_TRIANGLES);
    glVertex3f(   x1,    y2,   z2   );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f(     x2,    y2,   z1   );
    glEnd();

    // ROOF - Left
    glBegin(GL_TRIANGLES);
    glVertex3f( x1, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x1, y2, z1 );
    glEnd();

    // ROOF - Front
    glBegin(GL_TRIANGLES);
    glVertex3f( x1, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x2, y1, z2 );
    glEnd();
}

void drawCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat h)
{
      glColor3f( 0.0,  0.0, 0.0 );

      //side - FRONT
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );      // P1 is red
      glVertex3f( x1, y1, h  );      // P2 is green
      glVertex3f( x2, y1, h  );      // P3 is blue
      glVertex3f( x2, y1, z1 );      // P4 is purple

      glEnd();

      //side - BACK
      glBegin(GL_QUADS);
      glVertex3f( x1, y2, z2 );
      glVertex3f( x1, y2, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - RIGHT
      glBegin(GL_QUADS);
      glVertex3f( x2, y1, z1 );
      glVertex3f( x2, y1, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - LEFT
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );
      glVertex3f( x1, y1, h );
      glVertex3f( x1, y2, h );
      glVertex3f( x1, y2, z2 );
      glEnd();

      //side - TOP
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, h );
      glVertex3f( x1, y2, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y1, h );
      glEnd();

      //side - BOTTOM
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );
      glVertex3f( x1, y2, z1 );
      glVertex3f( x2, y2, z2 );
      glVertex3f( x2, y1, z2 );

      glEnd();
}

void drawHouse(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
    GLfloat normalizedZ1 = (z1 * MAX_HEIGHT)/255;
    GLfloat normalizedZ2 = (z2 * MAX_HEIGHT)/255;
    drawCube(x1, y1, normalizedZ1, x2, y2, normalizedZ2, 3.5);

    GLfloat centerX = x1 + ((x2 - (x1)) / 2);
    GLfloat centerY = y1 + ((y2 - (y1)) / 2);
    GLfloat centerZ = 5;
    // ROOF
    drawPyramid(x1, -2.5, 3.5, x2, -5.5, 3.5,  // TODO
                centerX, centerY, centerZ);

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

        glNormal3f(Cx,Cy,Cz);
        glVertex3f(Cx,Cy,Cz);
        for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
            z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
            y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
            glNormal3f(Cx,y,z);
            glVertex3f(Cx, y , z);
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
    Cx -= 0.2;

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
    drawCube(0.2f,-1.2f,1.0f, 1.0f, 6.2f,1.0f,2.5f);
    drawCube(0.2f,0.0f,3.5f, 1.0f, 4.0f,3.5f,2.0f);
    drawWindShield();
}

void drawCar(void) 
{
    draw_wheel(0.0f ,0.0f  ,1.0f);
    draw_wheel(1.0f ,0.0f  ,1.0f);
    draw_wheel(0.0f ,5.0f  ,1.0f);
    draw_wheel(1.0f ,5.0f  ,1.0f);
    drawCarBody();
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

        draw_plane();
        initLight();

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

void drawQuad(GLfloat x[], GLfloat y[], GLfloat z[])
{
	glBegin(GL_QUADS);
        glNormal3f(x[0],y[0],z[0]);
        glColor3f(1,0,0);
        glVertex3f(x[0], y[0], z[0]);

        glNormal3f(x[1],y[1],z[1]);
        glColor3f(1,1,0);
        glVertex3f(x[1], y[1], z[1]);

        glNormal3f(x[2],y[2],z[2]);
        glColor3f(1,0,0);
        glVertex3f(x[2], y[2], z[2]);

        glNormal3f(x[3],y[3],z[3]);
        glColor3f(1,0,0);
        glVertex3f(x[3], y[3], z[3]);
	glEnd();
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

        y[1] = (dl * distanceL -scene[0]/2.0f);
        x[1] = ((dw+1) * distanceW -scene[0]/2.0f);
        z[1] = ((MAX_HEIGHT*map[i+1])/255.0f);
        
        y[2] = ((dl+1) * distanceL -scene[0]/2.0f);
        x[2] = ((dw+1) * distanceW -scene[0]/2.0f);
        z[2] = ((MAX_HEIGHT*map[j+1])/255.0f);
        
        y[3] = ((dl+1) * distanceL -scene[0]/2.0f);
        x[3] = (dw * distanceW -scene[0]/2.0f);
        z[3] = ((MAX_HEIGHT*map[j])/255.0f);

        drawQuad(x,y,z);
    }
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

            if (i == 0)         { *width = stoi(s);  i++; } 
            else { if (i == 1)  { *legnth = stoi(s); i++;} 
                else                {(map)->push_back(stoi(s));
                }
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

void mydisplay(void)
{	
    // always make sure to be on MODEL_VIEW matric here!
    glLoadIdentity();

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    draw_axes();

    drawCar();
    drawHouse(house[0],house[1],house[2],
              house[3],house[4],house[5]);
    drawSun();
    buildPolygons();
    /*resetlight();*/

    glutSwapBuffers();
}

int main(int argc, char**argv) 
{
    glutInit(&argc, argv);

    // Parsing input files
    parseMap(&map,&width,&legnth);
    parseInit(&material,&house, &car, &sun);
    printInitVectors();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
    
    init();

    // Callback functions
	glutDisplayFunc(mydisplay);
	glutIdleFunc(rotateSun);
    glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
