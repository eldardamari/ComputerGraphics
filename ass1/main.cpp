#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>

#include <GLUT/glut.h>

using namespace std;


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
            if (i == 0) {*width = stoi(s);  i++; } 
            else { if (i == 1) { *legnth = stoi(s); i++;} 
                else {(map)->push_back(stoi(s));
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


int main(int argc, char**argv) {

    int clrTable[256][3];
    vector<int> map;
    vector<float> eye;

    int width,legnth;
    int i,j;

    /*GLdouble angleView;   
    GLdouble widthLenghtRelation;       
    GLdouble fNearPointOnGlanceV;       // farther distance of point located on glance vector.
    GLdouble dNearPointOnGlanceV;       // distance of nearest point on glance vector.
    GLdouble glanceX,glanceY,glanceZ;
    GLdouble eyeX,eyeY,eyeZ;
    GLdouble upX,upY,upZ;*/

    parseMap(&map,&width,&legnth);
    parseColorTable(clrTable);
    parseEye(&eye);

    /*for( std::vector<int>::const_iterator i = map.begin(); i != map.end(); ++i)
         std::cout << *i << ' ';

    for (i=0,j=0 ; i < 256 ;) {
        cout << clrTable[i][j] << " " ;
        j++;
        if (j== 3) {
            cout << endl;
            i++;
            j=0;
        }
    }
    
    for( std::vector<float>::const_iterator i = eye.begin(); i != eye.end(); ++i)
         std::cout << *i << ' ';
*/

    return 0;
}

