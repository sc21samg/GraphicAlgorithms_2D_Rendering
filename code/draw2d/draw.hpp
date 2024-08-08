#ifndef DRAW_HPP_BA97BA20_4B0E_45D8_97D4_65267FFA2EA6
#define DRAW_HPP_BA97BA20_4B0E_45D8_97D4_65267FFA2EA6

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 1!
//
// For CW1, the draw.hpp file must remain exactly as it is. In particular, you
// must not change any of the function prototypes in this header.

#include "forward.hpp"
#include "color.hpp"

#include "../vmlib/vec2.hpp"

// Coursework 1:
void draw_line_solid(
	Surface&,
	Vec2f aBegin, Vec2f aEnd,
	ColorU8_sRGB
);

void draw_triangle_solid(
	Surface&,
	Vec2f aP0, Vec2f aP1, Vec2f aP2,
	ColorU8_sRGB
);
void draw_triangle_interp(
	Surface&,
	Vec2f aP0, Vec2f aP1, Vec2f aP2,
	ColorF aC0, ColorF aC1, ColorF aC2
);

void draw_triangle_wireframe(
	Surface&,
	Vec2f aP0, Vec2f aP1, Vec2f aP2,
	ColorU8_sRGB
);

// From Exercise G.1
// You can ignore these in Coursework 1
void draw_rectangle_solid(
	Surface&,
	Vec2f aMinCorner, Vec2f aMaxCorner,
	ColorU8_sRGB
);

void draw_rectangle_outline(
	Surface&,
	Vec2f aMinCorner, Vec2f aMaxCorner,
	ColorU8_sRGB
);

#endif // DRAW_HPP_BA97BA20_4B0E_45D8_97D4_65267FFA2EA6
