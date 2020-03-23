#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <glut.h>

double iO = 0.05, jO = 0.07, iG = 0.01,kG = 0.02; // �������� �������� �����
float nO = 20, nG = 15; // ������ ����� �����
using namespace std;

enum faces //��� ����������� ����������
{
    XoY,
    XoZ,
    YoZ,
};

class point // ����� �����
{
private:
    double i; //���������� �����
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
}O(0,0,0), G(40,0,0); // 2 ����� ��� 2� �����

void DrawQuads(point O, float n, faces face) // ��������� ���������
{
    double I,J,K;
    I = O.GetI();   // ����� �������� ������� ��������
    J = O.GetJ();
    K = O.GetK();
    switch (face) // ���������� ���������, � ������� ����� ���������� �������
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

void DrawCube(point O, float n, unsigned int R,unsigned int G, unsigned int B) // ��������� ����
{

    point A;
    glColor3ub(0,0,0);
    A = O;
    A.PlusPoint(-n/2, "i"); // ��������� ������ �����
    DrawQuads(A, n, YoZ);

    A = O;
    A.PlusPoint(-n/2, "j"); // ��������� ����� �����
    DrawQuads(A, n, XoZ);

    A = O;
    A.PlusPoint(-n/2, "k"); // ��������� ������ �����
    DrawQuads(A, n, XoY);

    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "i"); // �������� ������ �����
    DrawQuads(A, n, YoZ);

    R -= 25; G -= 25; B -= 25;
    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "j"); // ��������� ������� �����
    DrawQuads(A, n, XoZ);

    R -= 25; G -= 25; B -= 25;
    glColor3ub(R,G,B);
    A = O;
    A.PlusPoint(n/2, "k");  // ��������� �������� �����
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
    glClearColor(1.0, 1.0, 1.0, 0.0); // ���� ���� - �����
    gluLookAt(20.0,10.0,30.0, 0.0,0.0,-10.0, 0.0,1.0,0.0); // ��������� ����� ������: ������ 3 ����� - ����� ������, ������ 3 ����� - ����� ���� ������� ������, ������ ��� ����� ������ ������
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0 , 100.0, -100.0, 100.0); // ������ ���������� ��� ������� ����� ������
    glViewport(0, 0, w, h);
}

void Coordinate()
{
    glBegin(GL_LINES);
    glVertex3d(0,0,-400); // ��� Z
    glVertex3d(0,0,400);

    glVertex3d(-400,0,0); // ��� Y
    glVertex3d(400,0,0);
    glEnd();
}

void MouseButton(int button, int state, int x, int y) // ��������� ������� ����
{
    if(button == GLUT_LEFT_BUTTON)  //����� ������
        if (state == GLUT_DOWN) nG+=1;
    if(button == GLUT_RIGHT_BUTTON) //������ ������
        if (state == GLUT_DOWN) nG-=1;
}
void RenderCartoon() //�������� �����
{
    glColor3d(0.0, 0.0, 0.0); //���� - ������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������� �������
    Coordinate();   //��������� ���������
        DrawCube(G, nG, 255 ,50, 50); //��������� �������� ����
        DrawCube(O, nO, 160 ,160, 160); //��������� ������ ����

        O.PlusPoint(iO, "i");   //�������� ������ ������ ����
        O.PlusPoint(jO, "j");
        if (abs(O.GetI()) >= 100.0)
            iO *= -1.0;
        if (abs(O.GetJ()) >= 100.0)
            jO *= -1.0;

        G.PlusPoint(iG, "i"); // �������� ������ �������� ����
        if (abs(O.GetI()) >= 40.0)
            iG *= -1.0;
    glutSwapBuffers();
}


int main (int argc, char **argv)
{
    glutInit(&argc, argv); //������������� GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //����� ����
    glutInitWindowSize(600, 600); // ������� ����
    glutInitWindowPosition(20, 20); // ������� ����
    glutCreateWindow("4laba");  // �������� ���� � ��������� 4laba
    //glutFullScreen();   //������������� �����
    glutDisplayFunc(RenderCartoon); //����������� ������� � ����
    glutIdleFunc(RenderCartoon); // ��� ������ ��������
    glutSpecialFunc(processSpecialKeys); // ��������� ���� ������
    glutReshapeFunc(ChangeSize); //��� ��������� ������� ���� ���������� ��� �������
    glutMouseFunc(MouseButton); //��������� ������� ����
    glutMainLoop(); //��� ��� ������ �� �������� :( (������� ���� ��������� ���� ���������� �������)
    return 0;
}
