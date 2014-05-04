#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else 
    #include <GL/glut.h>
#endif
#define GL_PI 3.1415926535897932384626433832795

using namespace std;

// Global db
vector<int> map;
vector<float> eye;
int colors[256][3];
int width,legnth;

void init()
{
	float modelMatrix[16], projectionMatrix[16];
	glClearColor(1, 1, 1, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

	gluPerspective(45, 1, 2, 25);
	gluLookAt(15.5,7.5, 8, 0.7,0.5,1, 0, 0, 1);  //define view direction
	
	//glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0); //For DEBUG

	//get matrices 
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void parseMap(vector<int> *map, int* width, int* legnth)
{
    int i = 0;

    ifstream file ( "./input/map.csv" );

    while (file.good())
    {
        string s;

        if (!getline(file,s)) break;
        istringstream ss(s);

        while(ss) 
        {
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

void parseColorTable(int clrTable[][3])
{
    int row = 0;
    int col = 0;
    int rowCounter = 1;

    ifstream file ( "./input/clrTable.csv" );

    while (file.good())
    {
        string s;

        if (!getline(file,s)) break;
        istringstream ss(s);

        while(ss) 
        {
            string s;

            if (!getline(ss,s,',')) break;

            clrTable[row][col] = stoi(s);
            col++;
            rowCounter++;

            if (rowCounter == 4) {
                rowCounter = 1;
                col = 0;
                row++;
            }
        }
    }
    file.close();
}

void parseEye(vector<float> *eye)
{
    ifstream file ( "./input/eye.csv" );

    while (file.good())
    {
        string s;

        if (!getline(file,s)) break;
        istringstream ss(s);

        while(ss) 
        {
            string s;

            if (!getline(ss,s,',')) break;
            else {
                (eye)->push_back(stoi(s));
            }
        }
    }
    file.close();
}

void drawQuad(float x[], float y[], float z[], int colorIdx[])
{
	glBegin(GL_QUADS);

	float red   = colors[colorIdx[0]][0];
	float green = colors[colorIdx[0]][1];
	float blue  = colors[colorIdx[0]][2];

	glColor3ub(red, green, blue);
	glVertex3f(x[0], y[0], z[0]);

	red     = colors[colorIdx[1]][0];
	green   = colors[colorIdx[1]][1];
	blue    = colors[colorIdx[1]][2];
	glColor3ub(red, green, blue);
	glVertex3f(x[1], y[1], z[1]);

	red     = colors[colorIdx[2]][0];
	green   = colors[colorIdx[2]][1];
	blue    = colors[colorIdx[2]][2];
	glColor3ub(red, green, blue);
	glVertex3f(x[2], y[2], z[2]);

	red     = colors[colorIdx[3]][0];
	green   = colors[colorIdx[3]][1];
	blue    = colors[colorIdx[3]][2];
	glColor3ub(red, green, blue);
	glVertex3f(x[3], y[3], z[3]);
	glEnd();

	glFlush(); //print to screen*/
}

void buildPolygons()
{
    int i = 0;          // first cell first row
    int j = width + 1;  // first cell second row
    float dw = 0;
    float dl = 0;
    float distanceW = 10.0/width;
    float distanceL = 10.0/legnth;

    GLfloat x[4] = {0,0,0,0};
    GLfloat y[4] = {0,0,0,0};
    GLfloat z[4] = {0,0,0,0};
    int color[4] = {0,0,0,0};


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

        x[0] = (float)(dl * distanceL -5.0);
        y[0] = (float)(dw * distanceW -5.0);
        z[0] = (float)((float)(2*map[i])/(float)255.0);
        color[0] = map[i];

        x[1] = (float)(dl * distanceL -5.0);
        y[1] = (float)((dw+1) * distanceW -5.0);
        z[1] = (float)((float)(2*map[i+1])/(float)255.0);
        color[1] = map[i+1];
        
        x[2] = (float)((dl+1) * distanceL -5.0);
        y[2] = (float)((dw+1) * distanceW -5.0);
        z[2] = (float)((float)(2*map[j+1])/(float)255.0);
        color[2] = map[j+1];
        
        x[3] = (float)((dl+1) * distanceL -5.0);
        y[3] = (float)(dw * distanceW -5.0);
        z[3] = (float)((float)(2*map[j])/(float)255.0);
        color[3] = map[j];

        drawQuad(x,y,z,color);
    }
}

void draw_axes(void)
{
float ORG[3] = {0,0,0};

float XP[3] = {5,0,0}, XN[3] = {-5,0,0},
      YP[3] = {0,5,0}, YN[3] = {0,-5,0},
      ZP[3] = {0,0,5}, ZN[3] = {0,0,-5};

    glLineWidth (2.0);
    glBegin (GL_LINES);

    glColor3f (1,0,0); // +X axis is red.
    glVertex3fv (ORG);
    glVertex3fv (XP ); 
    glColor3f (1,0,0); // -X axis is red.
    glVertex3fv (ORG);
    glVertex3fv (XN ); 
    
    glColor3f (0,1,0); // +Y axis is green.
    glVertex3fv (ORG);
    glVertex3fv (YP );
    glColor3f (0,1,0); // -Y axis is red.
    glVertex3fv (ORG);
    glVertex3fv (YN ); 

    glColor3f (0,0,1); // +Z axis is blue.
    glVertex3fv (ORG);
    glVertex3fv (ZP ); 
    glColor3f (0,0,1); // -Z axis is red.
    glVertex3fv (ORG);
    glVertex3fv (ZN ); 

    glEnd();

	glFlush(); //print to screen*/

}
void draw_wheel(void)
{
    GLfloat z, y,angle;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,0,0);

    glVertex3f(0.5,0,3);
    for (angle = 0.0f ; angle < 360 ; angle += 10.0f) {
        z = 3.0f*sin(angle*GL_PI/180) + 3.0f;
        y = 3.0f*cos(angle*GL_PI/180);
        cout << "y = " << y << " z = " << z << endl;
        glVertex3f(0.5, y , z);
    }
    glEnd();
    

    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);

    glVertex3f(0.5,0,3);
    glVertex3f(0.5, y , z);
    y = 3.0f*cos(0.0f);
    z = 3.0f*sin(0.0f) + 3.0f;
    glVertex3f(0.5, y , z);

    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,1,0);

    glVertex3f(-0.5,0,3);
    for (angle = 0.0f ; angle < 360 ; angle += 10.0f) {
        z = 3.0f*sin(angle*GL_PI/180) + 3.0f;
        y = 3.0f*cos(angle*GL_PI/180);
        cout << "y = " << y << " z = " << z << endl;
        glVertex3f(-0.5, y , z);
    }
    glEnd();
    
    glBegin(GL_TRIANGLES);

    glColor3f(0,1,0);

    glVertex3f(-0.5,0,3);
    glVertex3f(-0.5, y , z);
    y = 3.0f*cos(0.0f);
    z = 3.0f*sin(0.0f) + 3.0f;
    glVertex3f(-0.5, y , z);

    glEnd();

    glFlush();

}
void mydisplay(void)
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*buildPolygons();*/
    /*glRotatef(45,0.0,0.0,1.0);*/
    draw_axes();
    draw_wheel();


}

int main(int argc, char**argv) {

    // Parsing input files
    /*parseMap(&map,&width,&legnth);
    parseColorTable(colors);
    parseEye(&eye);*/

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Map");

	init();

	glutDisplayFunc(mydisplay);
    glutMainLoop();

    return 0;
}
