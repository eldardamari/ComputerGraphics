#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>

#include <GLUT/glut.h>

using namespace std;

// Global
vector<int> map;
vector<float> eye;
int colors[256][3];
int width,legnth;

void init()
{
	float modelMatrix[16], projectionMatrix[16];
	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

	gluPerspective(eye[0], eye[1], eye[2], eye[3]);
	gluLookAt(eye[4], eye[5], eye[6],eye[7],eye[8], eye[9], eye[10], eye[11], eye[12]);  //define view direction
	
	//glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0); //For DEBUG

	//get matrices 
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void parseMap(vector<int> *map, int* width, int* legnth)
{
    int i =0;

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

            if (i == 0)         {*legnth = stoi(s);  i++; } 
            else { if (i == 1)  { *width = stoi(s); i++;} 
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
    int i = 0;
    int j = width + 1;
    float dw = 0;
    float dl = 0;
    float distanceW = 10.0/(float)width;
    float distanceL = 10.0/(float)legnth;

    float x[4],y[4],z[4];
    int color[4];

    cout << "j: " << j << " i: " << i << endl;
    cout << "distanceW: " << distanceW << " distanceL: " << distanceL << endl;

    for( int iter = 1 ; j <= (int)map.size()-1 ; i++ , j++, dw++, iter++)
    {
        if (iter == width+1){
            cout << "i: " << i << " j: " << j << " dw: " << dw << " dl: " << dl << endl;
            i++;
            j++;
            dl++;
            dw=0;
            iter = 1;
        }

        /*if(i % legnth+1 == 0){
            cout << "row number: " << i+1 << " dl is: " << dl << endl;
            dl++;
            dw =0;
        }*/

        x[0] = (float)(dw * distanceW -5.0);
        y[0] = (float)(dl * distanceL -5.0);
        z[0] = (float)((float)(2*map[i])/(float)255.0);
        color[0] = map[i];

        x[1] = (float)((dw+1) * distanceW -5.0);
        y[1] = (float)(dl * distanceL -5.0);
        z[1] = (float)((float)(2*map[i+1])/(float)255.0);
        color[1] = map[i+1];
        
        x[2] = (float)((dw+1) * distanceW -5.0);
        y[2] = (float)((dl+1) * distanceL -5.0);
        z[2] = (float)((float)(2*map[j+1])/(float)255.0);
        color[2] = map[j+1];
        
        x[3] = (float)(dw * distanceW -5.0);
        y[3] = (float)((dl+1) * distanceL -5.0);
        z[3] = (float)((float)(2*map[j])/(float)255.0);
        color[3] = map[j];

        /*cout << z[0] << " " << z[1] << " " << z[2] << " " << z[3] << endl;*/
        /*cout << "color is :  " << color[0] << endl;
        cout << "color is :  " << color[1] << endl;
        cout << "color is :  " << color[2] << endl;
        cout << "color is :  " << color[3] << endl;*/
        drawQuad(x,y,z,color);
        
        /*cout << "legnth is : " << legnth << " dw: " << dw << " dl: " << dl << endl;
        cout << "legnth is : " << width << " dw: " << dw << " dl: " << dl << endl;*/

        /*cout << dw*distanceW-5 << " , " << dl*distanceL-5 << " , " << (map[i]) << endl;*/
        /*cout << (dw+1)*distanceW-5 << " , " << dl*distanceL-5 << " , " << (map[i+1]) << endl;*/
        /*cout << (dw+1)*distanceW-5 << "  ," << (dl+1)*distanceL-5 << " , " << (map[j+1]) << endl;*/
        /*cout << dw*distanceW-5 << "  ," << (dl+1)*distanceL-5 << " , " << (map[j]) << endl << endl;*/
    }
}

void mydisplay(void)
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buildPolygons();
}

int main(int argc, char**argv) {

    // Parsing input files
    parseMap(&map,&width,&legnth);
    parseColorTable(colors);
    parseEye(&eye);

    /*for ( vector<int>::iterator i = map.begin(); i != map.end() ; i++)
    {
        cout << *i << "<" << endl;
    }
    return 0;*/
	
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Map");

	init();

	glutDisplayFunc(mydisplay);
    glutMainLoop();

    return 0;
}
