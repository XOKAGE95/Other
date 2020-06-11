#include <cstring> //Подключение библиотек
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glut.h> //библиотека для отрисовки

using namespace std;

bool OpenImage(const char* filepath); //Прототипы функций
void keyboard(unsigned char key, int x, int y);
void display(void);

struct RGB // Структура для хранения цвета
{
    int   Blue;
    int   Green;
    int   Red;
};

class point // Структура точки
{
public:
    double i; //Координаты точки
    double j;
    RGB color; //Цвет точки

    point() : i(0),j(0) {};
    point (double I, double J)
    {
        i = I;
        j = J;
    };

    void PlusPoint(double A, std :: string coordinate)
    {
        if (coordinate == "i")
            i+= A;
        else if (coordinate == "j")
            j+= A;
    }
    void ReadColor(std :: ifstream & file)
    {
        file >> color.Red;
        file >> color.Green;
        file >> color.Blue;
        file.ignore(255, '\n');
    }

};

enum filter
{
    NO, // изображение без фильтра
    BW, // черно-белое, полутона
    MB, // повысить яркость
    MD, // понизить яркость
    NG, // негатив
    BN, // бинаризация (два цвета)
    UC, // повысить контрастность
    DC, // понизить контрастность
    GC // гамма коррекция
} choose(NO);

struct IMAGE //Структура картинки
{
    int width;
    int height;
    point *dots;
    int histR[256];
    int histG[256];
    int histB[256];
    unsigned int size;
} *bmp; //Указатель.


int main(int argc, char** argv)
{
                        //Считывание имени файла
    string stringname;
    cout << "Enter file name: ";
    cin >> stringname;

    char* charname = new char[stringname.length()];

    strcpy(charname, stringname.c_str()); //Конвертация string в char*

    if (!OpenImage(charname)) //Перенес алгоритм считывания данных с файла *.bmp из предыдущей лабораторной работы в функцию
        return 1;

                                    //Заполнения массива гистограммы цвета
    for (int k = 0; k < 256; k++)
    {
        bmp -> histR[k] = 0;
        bmp -> histG[k] = 0;
        bmp -> histB[k] = 0;

        for (int i = 0; i < bmp -> size; i++)
        {
            if (int(bmp -> dots[i].color.Red) == k)
                bmp -> histR[k] += (unsigned char)1;
            if (int(bmp -> dots[i].color.Green) == k)
                bmp -> histG[k] += (unsigned char)1;
            if (int(bmp -> dots[i].color.Blue) == k)
                bmp -> histB[k] += (unsigned char)1;
        }
    }

    glutInit(&argc, argv); //Инициализация GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //Режим дисплея
    glutInitWindowSize(1200, 650); //Размеры окна
    glutInitWindowPosition(0, 0); //Позиция окна
    glutCreateWindow("laba6"); //Названия окна

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, bmp -> width, 0.0, bmp -> height, -1.0, 1.0); //Координаты для крайних точек экрана 

    glutDisplayFunc(display); //Отображение функции 
    glutKeyboardFunc(keyboard); //Подключение клавиатуры
    glutMainLoop();
    return 0;
}

bool OpenImage(const char* name) //Функция считывания *.bmp файла
{
    bmp = NULL;
    FILE* file = fopen(name, "rb"); //Открытие файла
    if (file) //Если файл открылся
    {
        bmp = new IMAGE; //Выделение памяти
        bmp -> width = 0;
        bmp -> height = 0;
        bmp -> dots = NULL;

        unsigned char info[57]; //Считывание "мусорной информации"
        fread(info, sizeof(unsigned char), 57, file);
        bmp -> width = *(unsigned int*)&info[18]; //Размеры изображения
        bmp -> height = *(unsigned int*)&info[22];
        cout << "width = " << bmp -> width << "\theight = " << bmp -> height << endl;

        bmp -> size = bmp -> width * bmp -> height; //Количество пикселей

        bmp -> dots = new point[bmp -> size];//Создание массива точек

        unsigned char *colors = new unsigned char[bmp -> size*3]; //Массив для считывания цветов

        fread(colors, sizeof(unsigned char), bmp -> size*3, file); //Считывание цветов
        fclose(file);

        int o = 0; //Запись цветов в массив точек
        for (unsigned int i = 0; i < bmp -> size * 3; i += 3)
        {
            bmp -> dots[o].color.Blue = colors[i];
            bmp -> dots[o].color.Green = colors[i + 1];
            bmp -> dots[o].color.Red = colors[i + 2];
            o++;
        }
        o = 0;
        delete[] colors;

        for(int j = 0; j < bmp -> height; j++) //Запись координат точек
            for(int i = 0; i < bmp -> width; i++)
            {
                bmp -> dots[o].i = i;
                bmp -> dots[o].j = j;
                o++;
            }
        cout << "Size = " << bmp -> size/1024 << "KB" << endl;
        return true;
    }
    return false;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q': //Выход
        exit(0);
        break;
    case 's':
        cout << "No filter" << endl;
        choose = NO; // Без фильтра
        break;
    case 'z':
        cout << "Halftone filter" << endl;
        choose = BW; //Полутона
        break;
    case 'x':
        cout << "Lighter" << endl;
        choose = MB; // Светлее
        break;
    case 'c':
        cout << "Darker" << endl;
        choose = MD; // Темнее
        break;
    case 'v':
        cout << "Negative" << endl;
        choose = NG; // Негатив
        break;
    case 'b':
        cout << "Binarization" << endl;
        choose = BN; // Бинаризация
        break;
    case 'n':
        cout << "To increase the contrast" << endl;
        choose = UC; // Увеличить контрастность
        break;
    case 'm':
        cout << "To lower the contrast" << endl;
        choose = DC; // Уменьшить контрастность
        break;
    case 'g':
        cout << "Gamma correction" << endl;
        choose = GC; // Гамма корекция
        break;
    default:
        break;
    }
}

void display() //Функия отрисовки
{
    glClearColor(0.0, 0.0, 0.0, 0.0); //Цвет фона - черный
    glClear(GL_COLOR_BUFFER_BIT);
    double n = 1.0; //Размер ребра пикселя
    glBegin(GL_QUADS); //Режим отрисовки - квадраты

    switch (choose)
    {
    case NO:  // Без фильтров
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            glColor3ub(
                bmp -> dots[i].color.Red,
                bmp -> dots[i].color.Green,
                bmp -> dots[i].color.Blue);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }


    case BW: // Полутона
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            // используем формулу Y = 0.3R + 0.59G + 0.11B 
            unsigned char bw =
                bmp -> dots[i].color.Red * 0.3 +
                bmp -> dots[i].color.Green * 0.59 +
                bmp -> dots[i].color.Blue * 0.11;
            glColor3ub(bw, bw, bw);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }

    case MB: // Сделать ярче
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            if (bmp -> dots[i].color.Red < 250) // Ограничение выхода за границу отрезка [0;255]
                bmp -> dots[i].color.Red += (unsigned char)5; //Увеличение яркости
            if (bmp -> dots[i].color.Green < 250)
                bmp -> dots[i].color.Green += (unsigned char)5;
            if (bmp -> dots[i].color.Blue < 250)
                bmp -> dots[i].color.Blue += (unsigned char)5;
            glColor3ub(
                bmp -> dots[i].color.Red,
                bmp -> dots[i].color.Green,
                bmp -> dots[i].color.Blue);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case MD: // Сделать темнее
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            if (bmp->dots[i].color.Red > 5) // Ограничение выхода за границу отрезка [0;255]
                bmp->dots[i].color.Red -= (unsigned char)5; // Уменьшение яркости
            if (bmp->dots[i].color.Green > 5)
                bmp->dots[i].color.Green -= (unsigned char)5;
            if (bmp->dots[i].color.Blue > 5)
                bmp->dots[i].color.Blue -= (unsigned char)5;
            glColor3ub(
                bmp->dots[i].color.Red,
                bmp->dots[i].color.Green,
                bmp->dots[i].color.Blue);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case NG: // Негатив
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            glColor3ub(
                255 - bmp -> dots[i].color.Red, // Инвертируем цвет
                255 - bmp -> dots[i].color.Green,
                255 - bmp -> dots[i].color.Blue);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case BN: // Бинаризация
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            unsigned char bn1, bn2, bn3;
            if (bmp -> dots[i].color.Red > 127)
                bn1 = 240;
            else
                bn1 = 0;
            if (bmp -> dots[i].color.Green > 127)
                bn2 = 240;
            else
                bn2 = 0;
            if (bmp -> dots[i].color.Blue > 127)
                bn3 = 240;
            else
                bn3 = 0;
            glColor3ub(bn1, bn2, bn3);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case UC: // Повышение контрастности
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            unsigned char q1 = 60; // цвета со значением 60 будут иметь значение 0
            unsigned char q2 = 200; // а со значением 200 будут иметь значение 255
            unsigned char kf = 255 / (q2 - q1); // коэффициент домножения q2 элемента до 255 с учетом границы q1
            unsigned char uc1, uc2, uc3;
            if (bmp -> dots[i].color.Red < q1)
                uc1 = 0;
            else if (bmp -> dots[i].color.Red > q2)
                uc1 = 255;
            else
                uc1 = (bmp -> dots[i].color.Red - q1) * kf;

            if (bmp -> dots[i].color.Green < q1)
                uc2 = 0;
            else if (bmp -> dots[i].color.Green > q2)
                uc2 = 255;
            else
                uc2 = (bmp -> dots[i].color.Green - q1) * kf;

            if (bmp -> dots[i].color.Blue < q1)
                uc3 = 0;
            else if (bmp->dots[i].color.Blue > q2)
                uc3 = 255;
            else
                uc3 = (bmp->dots[i].color.Blue - q1) * kf;

            glColor3ub(uc1, uc2, uc3);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case DC: // Уменьшение контрастности
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            unsigned char q1 = 60; // Левая граница
            unsigned char q2 = 200; // Правая граница
            glColor3ub(
                q1 + bmp -> dots[i].color.Red * (q2 - q1) / 255,
                q1 + bmp -> dots[i].color.Green * (q2 - q1) / 255,
                q1 + bmp -> dots[i].color.Blue * (q2 - q1) / 255);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    case GC: // Гамма коррекция
    {
        for (int i = 0; i < bmp -> size; i++)
        {
            double gamma = 2;
            glColor3d(
                pow((double)bmp -> dots[i].color.Red / 255.0, gamma),
                pow((double)bmp->dots[i].color.Green / 255.0, gamma),
                pow((double)bmp->dots[i].color.Blue / 255.0, gamma));
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j);
            glVertex2d(bmp -> dots[i].i + 1, bmp -> dots[i].j + n);
            glVertex2d(bmp -> dots[i].i, bmp -> dots[i].j + n);
        }
        break;
    }
    default:
        break;
    }

    glEnd();
    glutSwapBuffers();

}

