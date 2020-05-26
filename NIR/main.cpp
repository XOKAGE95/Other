#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <math.h>
#include <locale.h>
#include "glut.h"

double lengthX;
double lengthY;
double x, xx;
double y, yy;
double z, zz;
int row = 0;
int col = 0;
double **arr;
std :: string *names;

using namespace std;

bool Parser();
int FileNum();
double stringToDouble(string A);
double average(double *arr, int n);
bool moda(double *arr, int n);
int found(double **arr, int n, double num);
bool distribution(double **count, int n, double *arr);
void cartoon(void);
void ChangeSize1(int w, int h);
void CoordinateSystem(void);
void renderBitmapString(float x, float y, float z, void *font, char *s);


int main(int argc, char **argv)
{
    setlocale(0,"");
    col = FileNum();
    cout << "Rows = " << row << endl;
    cout << "Columns = " << col << endl;
    Parser();

    for(int i = 0; i < col; i++)
        cout << setw(10) << left << names[i];
    cout << endl;

    /*for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
            cout << setw(7) << left << arr[j][i];
        cout << endl;
    }*/

    cout << "Average: " << endl;
    for(int i = 0; i < col; i++)
    {
        cout << setw(10) << left << average(arr[i], row);
    }

    cout << endl << "MODA: " << endl;
    for(int i = 0; i < col; i++)
        moda(arr[i], row);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(860, 620);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("heh");
    glutDisplayFunc(cartoon);
    glutReshapeFunc(ChangeSize1);
    glutMainLoop();

    for(int i = 0; i < col; i++)
        delete[] arr[i];
    delete[] arr;
    delete[] names;

    return 0;
}

bool Parser()
{
    ifstream file;
    file.open("Enter1.txt");
    if(!file) return -1;
    char symbol;
    int i = 0;
    int j = 0;

    names = new string[col];
    while(symbol != '\n')
    {
        symbol = file.get();
        if(symbol == '\n') break;
        if(symbol == ' ' || symbol == '\t')
        {
            j++;
            continue;
        }
        else
        {
            names[j]+= symbol;
        }

    }

    j = 0;
    string name;

    arr = new double*[col];
    for(int k = 0; k < col; k++)
        arr[k] = new double[row];

    while(!file.eof())
    {
        symbol = file.get();
        if(symbol == '\n')
        {
            arr[j][i] = stringToDouble(name);
            i++;
            j = 0;
            name = "";
            continue;
        }
        if(symbol == ' ' || symbol == '\t')
        {
            arr[j][i] = stringToDouble(name);
            j++;
            name = "";
            continue;
        }
        else
            name += symbol;
    }
    file.close();
    return true;
}

int FileNum()
{
    ifstream file;
    file.open("Enter1.txt");
    if(!file) return -1;
    int Collumns = 0;
    char ch;
    while(!file.eof())
    {
        ch = file.get();
        if(ch == ' ' || ch == '\t')
            Collumns++;
        if(ch == '\n')
        {
            row ++;
            Collumns++;
        }
    }
    row --;
    return Collumns/row;
}

double stringToDouble(string A)
{

    char *S = new char[A.length()];
    for(int i = 0; i < A.length(); i++)
        S[i] = A[i];
    int i,k = 1;
    double r=0,p=1;
    for (i=0; i< A.length(); i++)
    {
        if (k < 0) p=p*10;
        if (S[i] == '.')
            k=-k;
        else
            r=r*10+(S[i]-'0');
    }
    return r/p;
}

double average(double *arr, int n)
{
    double aver = 0;
    for(int i = 0; i < n; i++)
        aver += arr[i];
    return aver/n;
}

bool moda(double *arr, int n)
{
    double **count = new double*[n];
    for(int i = 0; i < n; i++)
        count[i] = new double[2];
    for(int i = 0; i < n; i++)
    {
        count[i][0] = -1;
        count[i][1] = 0;
    }
    distribution(count, n, arr);
    int max = 0;
    float index;
    for(int i = 0; i < n; i++)
    {
        if(count[i][1] > max)
        {
            max = count[i][1];
            index = count[i][0];
        }
    }
    string enter = "";
    enter = to_string(index);
    enter.erase(5, 100);
    int k = enter.find(",", 0);
    if(k != -1) enter[k] = '.';
    enter += "(" + to_string(max) + ")";
    cout << setw(10) << left << enter;
    return true;
}

int found(double **arr, int n, double num)
{

    for(int i = 0; i < n; i++)
    {
        if(arr[i][0] == num) return i;
    }
    return -1;
}

void cartoon(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    CoordinateSystem();
    glColor3ub(100, 100, 255);

    double **count = new double*[row];
    for(int i = 0; i < row; i++)
        count[i] = new double[2];
    for(int i = 0; i < row; i++)
    {
        count[i][0] = -1;
        count[i][1] = 0;
    }

    distribution(count, row, arr[1]);
    cout << endl << setw(9) << "Number" << setw(10) << "count" << endl;
    glBegin(GL_LINES);
    for(int i = 1; i < row; i++)
    {
        if(count[i][1] == 0) continue;
        cout << count[i-1][0] << "\t" << count[i-1][1] << endl;
        glVertex2d(i - 1, count[i-1][1]);
        glVertex2d(i, count[i][1]);
    }
    glEnd();

    glColor3ub(0, 0, 0);
    for(int i = 0; i < row; i += 2)
    {
        if(count[i][1] == 0) continue;
        string a = to_string(count[i][0]);
        a.erase(5, 100);
        int k = a.find(",", 0);
        if(k != -1) a[k] = '.';
        char c[10];
        strcpy(c, a.c_str());
        renderBitmapString(i - lengthX, 0 - lengthY * 5, 0, GLUT_BITMAP_TIMES_ROMAN_10, c);

        if(count[i + 1][1] == 0) continue;
        a = to_string(count[i + 1][0]);
        a.erase(5, 100);
        k = a.find(",", 0);
        if(k != -1) a[k] = '.';
        c[10];
        strcpy(c, a.c_str());
        renderBitmapString(i + lengthX, 0 + lengthY * 5, 0, GLUT_BITMAP_TIMES_ROMAN_10, c);
    }

    for(int i = 1; i < yy; i++)
    {
        string a = to_string(i);
        char c[10];
        strcpy(c, a.c_str());
        renderBitmapString(0 - lengthX * 3, i - lengthY, 0, GLUT_BITMAP_TIMES_ROMAN_10, c);
    }

    glutSwapBuffers();
}

void ChangeSize1(int w, int h)
{
    x = -2.0;
    xx = 50.0;
    y = -1.0;
    yy = 10.0;
    z = -100.0;
    zz = 100.0;
    if(h == 0)
        h = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(x, xx, y, yy, z, zz);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool distribution(double **count,int n, double *arr)
{
    int k =0;

    for(int i = 0; i < n; i++)
    {
        int num = found(count, n, arr[i]);
        if(num == -1)
        {
            count[k][0] = arr[i];
            count[k][1] = 1;
            k++;

        }
        else count[num][1] ++;
    }
    return true;
}

void CoordinateSystem()
{
    lengthX = abs((xx - x)/150);
    lengthY = abs((yy - y)/150);
    glLineWidth(1.0);
    glColor3d(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(x, 0);
    glVertex2d(xx, 0);
    glVertex2d(0, y);
    glVertex2d(0, yy);
    for(int i = x; i < xx; i++)
    {
        glVertex2d(i, lengthY);
        glVertex2d(i, -lengthY);
    }
    for(int i = y; i < yy; i++)
    {
        glVertex2d(lengthX, i);
        glVertex2d(-lengthX, i);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(xx, 0);
    glVertex2d(xx - lengthX*4, lengthY*2);
    glVertex2d(xx - lengthX*4, -lengthY*2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2d(0, yy);
    glVertex2d(lengthX*2, yy - lengthY*4);
    glVertex2d(-lengthX*2, yy - lengthY*4);
    glEnd();

    glRasterPos3f(xx - lengthX * 4, 0 + lengthY * 4, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');

    glRasterPos3f(0 + lengthX * 4, yy - lengthY * 4, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
}

void renderBitmapString(float x, float y, float z, void *font, char *s)
{

	char *c;
	glRasterPos3f(x, y,z);
	for (c = s; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}

