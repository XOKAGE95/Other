#include <iostream>
#include <cmath>
#include "GL/glut.h"
using namespace std;
#define PI 3.1415
int width = 0; // глобальные переменные для хранения значений ширины и высоты окна
int height = 0;

// алгоритм Брезенхема растровой дискретизации прямой 
void bresenham_line(int x1, int y1, int x2, int y2) { // передаем x;y значения двух точек, которые надо соединить


	int A = y2 - y1;
	int B = x1 - x2;
	int signA;
	int signB;
	A < 0 ? signA = -1 : signA = 1;
	B < 0 ? signB = -1 : signB = 1;
	int f = 0; // переменная для хранения изменения отклонения
	int x = x1;
	int y = y1;
	glVertex2d(x, y);
	/* от начальной точки можно сделать два шага - либо по вертикали (по горизонтали), либо по диагонали на 1 пиксель. */
	if (fabs(A) < fabs(B)) { // горизонталь или диагональ
		do {
			f += A * signA;
			if (f > 0) { // проверка на изменение отклонения
				f -= B * signB;
				y += signA;
			}
			x -= signB;
			glVertex2d(x, y);
		} while (x != x2 || y != y2); // пока не дойдем до точки х2 или у2
	}
	else { // вертикаль или диагональ, аналогично верхней функции, но меняем x и y местами
		do {
			f += B * signB;
			if (f > 0) {
				f -= A * signA;
				x -= signB;
			}
			y += signA;
			glVertex2d(x, y);
		} while (x != x2 || y != y2);
	}
}

void bresenham_ellipse(int rx, int ry, int xc, int yc) {
	double dx, dy, d1, d2, x, y;
	x = 0;
	y = ry;
	d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
	dx = 2 * ry * ry * x; // градиент по x
	dy = 2 * rx * rx * y; // градиент по y
	/* первая область, верхняя часть */
	while (dx < dy) {
		// выводим точки симметрично и с учетом центра 
		glVertex2d(x + xc, y + yc);
		glVertex2d(-x + xc, y + yc);
		glVertex2d(x + xc, -y + yc);
		glVertex2d(-x + xc, -y + yc);
		// используем условный параметр
		if (d1 < 0) {
			x++;
			dx = dx + (2 * ry * ry);
			d1 = d1 + dx + (ry * ry);
		}
		else {
			x++;
			y--;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d1 = d1 + dx - dy + (ry * ry);
		}
	}
	// условный параметр для второгшо региона 
	d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
		((rx * rx) * ((y - 1) * (y - 1))) -
		(rx * rx * ry * ry);
	// вторая область, нижняя часть 
	while (y >= 0) {
		glVertex2d(x + xc, y + yc);
		glVertex2d(-x + xc, y + yc);
		glVertex2d(x + xc, -y + yc);
		glVertex2d(-x + xc, -y + yc);
		if (d2 > 0) {
			y--;
			dy = dy - (2 * rx * rx);
			d2 = d2 + (rx * rx) - dy;
		}
		else {
			y--;
			x++;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d2 = d2 + dx - dy + (rx * rx);
		}
	}
}

void draw_pentagon(int r, int xc, int yc) {
	double pent[5][2]; // двумерный массив т.к. задаем точки параметрически 
	double da = 2 * PI / 5.0; // значения передаем в радианах
	for (int v = 0; v < 5; v++) {
		double angle = v * da;
		pent[v][0] = r * cos(angle) + xc; // xc и yc устанавливают центр
		pent[v][1] = r * sin(angle) + yc;
	}
	glColor3f(1.0, 1.0, 1.0); // цвет границы
	glBegin(GL_POLYGON);
	for (int v = 0; v < 5; v++) // строим точки
		glVertex2dv(pent[v]);
	glEnd();

	for (int v = 0; v < 5; v++) {
		double angle = v * da;
		pent[v][0] = (r - 2) * cos(angle) + xc;
		pent[v][1] = (r - 2) * sin(angle) + yc;
	}
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (int v = 0; v < 5; v++)
		glVertex2dv(pent[v]);
	glEnd();
}

/* функция для получения цвета пикселя по заданным координатам
 с условием координатной сетки */
void get_pixel_color(int x, int y, unsigned char* arr) {
	x += width / 2;
	y += height / 2;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, arr);
}

/* проверка, является ли пиксель по указанным координатом цвета границы */
bool pixel_is_border(const unsigned char* border, int x, int y) {
	unsigned char clr_temp[4];
	get_pixel_color(x, y, clr_temp);
	for (int i = 0; i < 3; i++) {
		if ((int)clr_temp[i] != (int)border[i])
			return false;
	}
	return true;
}

void color(const unsigned char* border, const unsigned char* fill, int x, int y) {
	int x_step = x; // координаты текущего пикселя
	int y_step = y;
	while (!pixel_is_border(border, x_step, y_step)) // пока слева не граница
		x_step -= 1; // двигаемся влево
	glPointSize(1); // устанавливаем размер 1 пиксель
	glColor4ubv(fill); // цвет заполения
	bool flag = true; // условие выхода
	while (flag) { // пока не достигли условия выхода
	/* пока справа не граница, идем вправо и заполняем цветом. */
		while (!pixel_is_border(border, x_step, y_step)) {
			glBegin(GL_POINTS);
			glVertex2d(x_step, y_step);
			x_step += 1;
			glEnd();
		}
		while (pixel_is_border(border, x_step, y_step - 1)) { // пока пиксель внизу - граница
			if (pixel_is_border(border, x_step - 1, y_step)) { // если мы дошли до границы слева, то значит мы не смогли спуститься,
				flag = false; // закрасили.
				break;
			}
			x_step--;
		}
		if (!flag) // проверка на условие выхода из цикла
			break;
		y_step--; // спускаемся на единицу вниз
		while (!pixel_is_border(border, x_step + 1, y_step)) // то мы идем вправо до упора.
			x_step++;
		while (!pixel_is_border(border, x_step, y_step)) {
			glBegin(GL_POINTS);
			glVertex2d(x_step, y_step);
			x_step--;
			glEnd();
		}
		while (pixel_is_border(border, x_step, y_step - 1)) { // пока пиксель внизу это граница
			if (pixel_is_border(border, x_step + 1, y_step)) { // если мы дошли до границы, то значит закрасили все.
				flag = false; // аналогично условие выхода
				break;
			}
			x_step++;
		}
		if (!flag)
			break;
		y_step--;
		while (!pixel_is_border(border, x_step - 1, y_step)) // пока слева не граница
			x_step--; // идем влева до упора, переходим с след итерации
	}
}

// эта функция служит для определения RGB значений пикселя
void motion(int x, int y) { // x,y - позиция указателя
// точка (0; 0) курсора находится сверху слева
	y = height - y;
	unsigned char pixel[4]; // выводим только RGB
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	cout << "[" << x - width / 2 << "; " << y - height / 2 << "]" << endl; // выводим с учетом координатной сетки
	cout << "R: " << (int)pixel[0] << endl;
	cout << "G: " << (int)pixel[1] << endl;
	cout << "B: " << (int)pixel[2] << endl;
	cout << endl;
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	unsigned char fill[4] = { 150, 200, 65 }; // цвет заполнения пятиугольника и цвет эллипса и прямой брезенхема
	unsigned char border[4] = { 255, 255, 255 }; // цвет границы пятиугольника
	glPointSize(1);
	glBegin(GL_POINTS); // рисуем по точкам
	glColor4ubv(fill); // указываем цвет
	bresenham_ellipse(80, 65, 100, 170); // в эллипс передаем радиус по оси Х, радиус по оси Y, и координаты центра
	bresenham_line(-20, 5, 100, 30); // в прямую передаем x1,y1 и x2,y2 точки, которые будем соединять.
	glEnd();
	draw_pentagon(60, 60, -60); // рисуем пятиугольник
	color(border, fill, 77, -5); // верхняя незакрашення граница в точке (77; -5)
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv) 
{
	setlocale(LC_ALL,"ru");
	cout << "Введите размер окна.\n" // задаем с клавиатуры размеры окна
		<< "Ширина: ";
	cin >> width;
	cout << "Высота: ";
	cin >> height;
	glutInit(&argc, argv); // инициализируем glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // использовать 2 буфера и цвет RGB
	glutInitWindowSize(width, height); // размер окна
	glutInitWindowPosition(0, 0); // позиция открытия окна в левом верхнем углу
	glutCreateWindow("ЛР7"); // имя окна
	glClearColor(0.0, 0.0, 0.0, 0.0); // цвет фона
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// координатная сетка
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1.0, 1.0);
	glutDisplayFunc(display); // запуск функции отображения
	glutPassiveMotionFunc(motion); // функция для считывания позиции курсора
	glutMainLoop();
	return 0;
}