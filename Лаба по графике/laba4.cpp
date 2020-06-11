#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <glut.h>

double iO = 0.05, jO = 0.07, iG = 0.01,kG = 0.02; // Скорости смещения кубов
float nO = 20, nG = 15; // Размер ребра кубов
using namespace std;

enum faces //для обозначения плоскостей
{
    XoY,
    XoZ,
    YoZ,
};

class point // Класс точки
{
private:
    double i; //Координаты точки
    double j;
    double k;
public:
    point() : i(0),j(0),k(0) {};
    point (double I, double J, double K)
    {
        i = I;
        j = J;
        k = K;
    };

    double GetI() {return i;};
    double GetJ() {return j;};
    double GetK() {return k;};
    void SetI(double I) {i = I;};
    void SetJ(double J) {j = J;};
    void SetK(double K) {k = K;};
    void PlusPoint(double A, string coordinate)
    {
        if (coordinate == "i")
            i+= A;
        else if (coordinate == "j")
            j+= A;
        else if (coordinate == "k")
            k+= A;
    }
}O(0,0,0), G(40,0,0); // 2 точки для 2х кубов

void DrawQuads(point O, float n, faces face) // Отрисовка квадратов
{
    double I,J,K;
    I = O.GetI();   // Точка является центром квадрата
    J = O.GetJ();
    K = O.GetK();
    switch (face) // Нахождение плоскости, в которой будет рисоваться квадрат
    {
    case XoY:
        I += -n/2;
        J += -n/2;
        glBegin(GL_QUADS);
        glVertex3d(I, J, K);
        glVertex3d(I, J + n, K);
        glVertex3d(I + n, J + n, K);
        glVertex3d(I + n, J, K);
        glEnd();
        break;
    case XoZ:
        K += -n/2;
        I += -n/2;
        glBegin(GL_QUADS);
        glVertex3d(I, J, K);
        glVertex3d(I, J, K + n);
        glVertex3d(I + n, J, K + n);
        glVertex3d(I + n, J, K);
        glEnd();
        break;
    case YoZ:
        K += -n/2;
        J += -n/2;
        glBegin(GL_QUADS);
        glVertex3d(I, J, K);
        glVertex3d(I, J, K + n);
        glVertex3d(I, J + n, K + n);
        glVertex3d(I, J + n, K);
        glEnd();
        break;
    }

}

void DrawCube(point O, float n, unsigned int R,unsigned int G, unsigned int B) // Отрисовка куба
{

    point A;
    glColor3ub(0,0,0);
    A = O;
    A.PlusPoint(-n/2, "i"); // Отрисовка задней грани
    DrawQuads(A, n, YoZ);

    A = O;
    A.PlusPoint(-n/2, "j"); // Отрисовка левой грани
    DrawQuads(A, n, XoZ);

    A = O;
    A.PlusPoint(-n/2, "k"); // Отрисовка нижней грани
    DrawQuads(A, n, XoY);

    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "i"); // Орисовка правой грани
    DrawQuads(A, n, YoZ);

    R -= 25; G -= 25; B -= 25;
    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "j"); // Отрисовка Верхней грани
    DrawQuads(A, n, XoZ);

    R -= 25; G -= 25; B -= 25;
    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "k");  // Отрисовка передней грани
    DrawQuads(A, n, XoY);
}

void processSpecialKeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
            glRotated(3, 0, 0, 1);
            glutPostRedisplay();
                        break;
		case GLUT_KEY_RIGHT:
            glRotated(-3, 0, 0, 1);
            glutPostRedisplay();
                        break;
		case GLUT_KEY_UP:
		    glTranslatef(0, 0.5, 0);
		    glutPostRedisplay();
                        break;
        case GLUT_KEY_DOWN:
            glTranslatef(0, -0.5, 0);
		    glutPostRedisplay();
                        break;
	}
}

void ChangeSize(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;
    glClearColor(1.0, 1.0, 1.0, 0.0); // Цвет фона - белый
    gluLookAt(20.0,10.0,30.0, 0.0,0.0,-10.0, 0.0,1.0,0.0); // Изменение точки обзора: первые 3 точки - точка обзора, вторые 3 точки - точка куда смотрит камера, третьи три точки наклон камеры
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0 , 100.0, -100.0, 100.0); // Задает координаты для крайних точек экрана
    glViewport(0, 0, w, h);
}

void Coordinate()
{
    glBegin(GL_LINES);
    glVertex3d(0,0,-400); //  Ось Z
    glVertex3d(0,0,400);

    glVertex3d(-400,0,0); // Ось Y
    glVertex3d(400,0,0);
    glEnd();
}

void MouseButton(int button, int state, int x, int y) // Обработка нажатий мыши
{
    if(button == GLUT_LEFT_BUTTON)  // Левая кнопка
        if (state == GLUT_DOWN) nG+=1;
    if(button == GLUT_RIGHT_BUTTON) // Правая кнопка
        if (state == GLUT_DOWN) nG-=1;
}
void RenderCartoon() // Правая кнопка
{
    glColor3d(0.0, 0.0, 0.0); // цвет - черный
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буффера
    Coordinate();   // Отрисовка координат
        DrawCube(G, nG, 255 ,50, 50); // Отрисовка красного куба
        DrawCube(O, nO, 160 ,160, 160); // Отрисовка серого куба

        O.PlusPoint(iO, "i");   // Смещение центра серого куба
        O.PlusPoint(jO, "j");
        if (abs(O.GetI()) >= 100.0)
            iO *= -1.0;
        if (abs(O.GetJ()) >= 100.0)
            jO *= -1.0;

        G.PlusPoint(iG, "i"); // Смещение центра красного куба
        if (abs(O.GetI()) >= 40.0)
            iG *= -1.0;
    glutSwapBuffers();
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv); // Инициализация GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Режим окна
    glutInitWindowSize(600, 600); // Размеры окна
    glutInitWindowPosition(20, 20); // Позиция окна
    glutCreateWindow("4laba");  // Создание окна с названием 4laba
    //glutFullScreen();   // Полноэкранный режим
    glutDisplayFunc(RenderCartoon); // Отоброжение функции в окне
    glutIdleFunc(RenderCartoon); // Для работы анимации
    glutSpecialFunc(processSpecialKeys); // Обработка спец клавиш
    glutReshapeFunc(ChangeSize); // При изменении размера окна вызывается эта функция
    glutMouseFunc(MouseButton); // При изменении размера окна вызывается эта функция
    glutMainLoop(); // Без нее ничего не работает :( (Создает цикл обработки всех предыдущих функций)
    return 0;
}
