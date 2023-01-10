#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/freeglut.h>

#include "shape.h"

struct Shape
{
	Color_t color;
	enum ShapeDraw draw_type;

	unsigned vertex_count;
	Point_t vertices[];
};

Shape_t* shape_new(
	const Color_t color,
	const enum ShapeDraw draw_type,
	const unsigned vertex_count,
	const Point_t vertices[static vertex_count])
{
	const size_t array_size = vertex_count * sizeof *vertices;

	Shape_t* const sh = malloc(sizeof *sh + array_size);

	*sh = (Shape_t) {
		.color = color,
		.draw_type = draw_type,
		.vertex_count = vertex_count,
	};

	memcpy(sh->vertices, vertices, array_size);
	return sh;
}

void shape_delete(Shape_t** shape)
{
	free(*shape);
	*shape = NULL;
}

void shape_draw(const Shape_t* shape)
{
	glColor3dv(shape->color.c);

	switch (shape->draw_type)
	{
	case SHAPE_OPEN:
		glBegin(GL_LINE_STRIP);
		break;
	case SHAPE_OUTLINE:
		glBegin(GL_LINE_LOOP);
		break;
	case SHAPE_FILLED:
		glBegin(GL_POLYGON);
		break;
	}

	for (unsigned i = 0; i < shape->vertex_count; ++i)
	{
		glVertex2dv(shape->vertices[i].v);
	}

	if (shape->draw_type != SHAPE_OPEN)
	{
		glEnd();
	}
}

static Shape_t* processed_shape = NULL;

static const ShapeFunctions_t* f_translate(double x, double y);
static const ShapeFunctions_t* f_scale(double x, double y);
static const ShapeFunctions_t* f_mirror(bool flipX, bool flipY);
static const ShapeFunctions_t* f_rotate(double theta);
static const ShapeFunctions_t* f_set_color(Color_t new_color);
static const ShapeFunctions_t* f_draw(void);

static const ShapeFunctions_t* functions = &(ShapeFunctions_t){
	.translate = &f_translate,
	.scale = &f_scale,
	.mirror = &f_mirror,
	.rotate = &f_rotate,

	.set_color = &f_set_color,

	.draw = &f_draw,
};

static const ShapeFunctions_t* f_translate(const double x, const double y)
{
	for (unsigned i = 0; i < processed_shape->vertex_count; ++i)
	{
		double* const coords = processed_shape->vertices[i].v;
		coords[X] += x;
		coords[Y] += y;
	}

	return functions;
}

static const ShapeFunctions_t* f_scale(const double x, const double y)
{
	for (unsigned i = 0; i < processed_shape->vertex_count; ++i)
	{
		double* const coords = processed_shape->vertices[i].v;
		coords[X] *= x;
		coords[Y] *= y;
	}

	return functions;
}

static const ShapeFunctions_t* f_mirror(const bool x, const bool y)
{
	for (unsigned i = 0; i < processed_shape->vertex_count; ++i)
	{
		double* const coords = processed_shape->vertices[i].v;
		coords[X] *= (x ? -1 : 1);
		coords[Y] *= (y ? -1 : 1);
	}

	return functions;
}

static const ShapeFunctions_t* f_rotate(const double theta)
{
	for (unsigned i = 0; i < processed_shape->vertex_count; ++i)
	{
		double* const coords = processed_shape->vertices[i].v;

		const double newX = coords[X] * cos(theta) - coords[Y] * sin(theta);
		const double newY = coords[X] * sin(theta) + coords[Y] * cos(theta);

		coords[X] = newX;
		coords[Y] = newY;
	}

	return functions;
}

static const ShapeFunctions_t* f_set_color(const Color_t new_color)
{
	processed_shape->color = new_color;
	return functions;
}

static const ShapeFunctions_t* f_draw(void)
{
	shape_draw(processed_shape);
	return functions;
}

const ShapeFunctions_t* with_shape(Shape_t* shape)
{
	processed_shape = shape;
	return functions;
}