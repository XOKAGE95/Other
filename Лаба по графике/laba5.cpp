#include <glut.h>
#include <iostream>
#include <fstream>
#include <io.h>

#include "point.h"
//#include "File.h"

using namespace std;

struct bmp
{
    int height;
    int width;
    point *dots;
    int size;
} *image;



void DrawQuads(point);
void ChangeSize(GLsizei w, GLsizei h);
void Cartoon();

int main(int argc, char **argv)
{
    image = new bmp;
    FILE *file = fopen("image.bmp", "rb");
    if (file)
    {
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 57, file); // Считываение 54 символов из потока file
        image -> width = *(unsigned int*)&info[18]; // ширина изображения
        image -> height = *(unsigned int*)&info[22]; //высота изображения
        cout << "width = " << image -> width << "\theight = " << image -> height << endl;
        image -> size =  (image -> width) * (image -> height); // Сколько памяти необходимо для хранения изображения
        image -> dots = new point[image -> size];
        unsigned char *color = new unsigned char[image -> size*3];
        fread(color, sizeof(unsigned char), image -> size*3, file);
        fclose(file);
        int o = 0;
        for (int i = 0; i < image -> size*3; i += 3)
        {
            //cout << (int)color[i] << " " << (int)color[i+1] << " " << (int)color[i+2] << endl;
            image -> dots[o].color.Blue = color[i];
            image -> dots[o].color.Green = color[i+1];
            image -> dots[o].color.Red = color[i+2];
            o++;
        }
        o = 0;
       for(int j = 0; j < image -> height; j++)
        for(int i = 0; i < image -> width; i++)
            {
                image -> dots[o].i = i;
                image -> dots[o].j = j;
                o++;
            }

        cout << image -> size;
    }

    else
        return -1;
    fclose(file);
    glutInit(&argc, argv); // Инициализация GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Режим окна
    glutInitWindowSize(image -> width,image -> height); // Размеры окна
    glutInitWindowPosition(20, 20); // Позиция окна
    glutCreateWindow("4laba");  // Создание окна с названием 4laba
    glutDisplayFunc(Cartoon);
    glutReshapeFunc(ChangeSize);
    glutMainLoop();
    return 0;
}

void Cartoon()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_QUADS);
    for(int i = 0; i < image -> size; i++)
    {
        glColor3ub(image -> dots[i].color.Red, image -> dots[i].color.Green, image -> dots[i].color.Blue);
        glVertex2d(image -> dots[i].i, image -> dots[i].j);
        glVertex2d(image -> dots[i].i + 1, image -> dots[i].j);
        glVertex2d(image -> dots[i].i + 1, image -> dots[i].j + 1);
        glVertex2d(image -> dots[i].i, image -> dots[i].j + 1);
    }
    glEnd();
    glutSwapBuffers();
}

void DrawQuads(point O) // Отрисовка квадратов
{
    int n = 1;
    double I,J,K;
    I = O.i;  // Точка является центром квадрата
    J = O.j;
    glColor3ub(O.color.Red, O.color.Green, O.color.Blue);
    glVertex2d(I, J);
    glVertex2d(I, J + n);
    glVertex2d(I + n, J + n);
    glVertex2d(I + n, J);

}

void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(0, image -> width, 0, image -> height, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
