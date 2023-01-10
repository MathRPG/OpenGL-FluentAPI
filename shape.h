#ifndef SHAPE_H
#define SHAPE_H

#include <stdbool.h>

typedef enum
{
	X = 0,
	Y = 1,
} Axis_t;

typedef struct Point
{
	double v[2];
} Point_t;

typedef enum
{
	R = 0,
	G = 1,
	B = 2,
} Channel_t;

typedef struct Color
{
	double c[3];
} Color_t;

typedef struct Shape Shape_t;

enum ShapeDraw
{
	SHAPE_OPEN, // Must call glEnd() externally
	SHAPE_OUTLINE,
	SHAPE_FILLED,
};

Shape_t* shape_new(Color_t color, enum ShapeDraw draw_type, unsigned vertex_count, const Point_t vertices[]);
void shape_delete(Shape_t** shape);

void shape_draw(const Shape_t* shape);

typedef struct ShapeFunctions ShapeFunctions_t;

const ShapeFunctions_t* with_shape(Shape_t* shape);

struct ShapeFunctions
{
	const ShapeFunctions_t* (* translate)(double x, double y);
	const ShapeFunctions_t* (* scale)(double x, double y);
	const ShapeFunctions_t* (* mirror)(bool x, bool y);
	const ShapeFunctions_t* (* rotate)(double theta);

	const ShapeFunctions_t* (* set_color)(Color_t new_color);

	const ShapeFunctions_t* (* draw)(void);
};

#endif // SHAPE_H