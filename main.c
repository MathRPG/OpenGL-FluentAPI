#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/freeglut.h>

#include "shape.h"

void display(void);

int main(int argc, char** const argv)
{
	printf("Initializing...\n");

	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(600, 600);

	glutCreateWindow("First Program.c");

	glutDisplayFunc(display);

	glutMainLoop();

	printf("Finished!\n");
	return 0;
}

void draw(void);

double random_double();
void display(void)
{
	glClearColor(1, 1, 1, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1000, 1000, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();

	glutSwapBuffers();
}

Color_t random_color(void)
{
	static double r = 0;
	static double g = 0;
	static double b = 0;

	static double r_inc = 0;
	static double g_inc = 0;
	static double b_inc = 0;

	if (r_inc == 0)
		r_inc = random_double() / 5;

	if (g_inc == 0)
		g_inc = random_double() / 5;

	if (b_inc == 0)
		b_inc = random_double() / 5;

	r += r_inc;
	g += g_inc;
	b += b_inc;

	if (r > 1 || r < 0)
		r_inc *= -1;

	if (g > 1 || g < 0)
		g_inc *= -1;

	if (b > 1 || b < 0)
		b_inc *= -1;

	return (Color_t){
		.c = {
			r, g, b
		},
	};
}

double random_double()
{
	return rand() / (double)RAND_MAX;
}

#define PI 3.1416

void draw(void)
{
	const Point_t points[] = {
		{ 0, 0 },
		{ 1, 0 },
		{ .5, sqrt(3) / 2 },
		{ 1, 1 },
		{ 0, 1 },
	};

	__auto_type shape = shape_new(
		(Color_t){ 0, 0, 0 },
		SHAPE_FILLED,
		sizeof points / sizeof *points,
		points
	);

	with_shape(shape)->scale(2000, 2000);

	for (int i = 0; i < 10000; ++i)
	{
		with_shape(shape)
			->rotate(PI / 20)
			->set_color(random_color())
			->scale(0.999, 0.999)
			->draw()
			/**/;
	}

	glEnd();

	shape_delete(&shape);
}