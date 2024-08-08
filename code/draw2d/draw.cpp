#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

//void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
//{
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//
//	//TODO: remove the following when you start your implementation
//	(void)aSurface; // Avoid warnings about unused arguments until the function
//	(void)aBegin;   // is properly implemented.
//	(void)aEnd;
//	(void)aColor;
//}

void draw_line_solid(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
{
	//TODO: your implementation goes here
	//task 1.2

	//build line between aBegin and aEnd
	//make line as thin as possible: single pixel width 
	//use parametric equation
	//use clipping
	//pick drawing method: bresenham's line drawing algorithm
	//method must do O(N) scaling


	// we calculate the differences in x and y coordinates between the two points
	// to get the length of our line/vector
	//abs() return absolute positive int
	//we decide the direction of the line in the next step


	int coord_length_x = abs(static_cast<int>(aEnd.x) - static_cast<int>(aBegin.x));
	int coord_length_y = abs(static_cast<int>(aEnd.y) - static_cast<int>(aBegin.y));

	// we need to determine the direction of the line/vector (1 or -1 for each axis)
	int direction_x, direction_y;

	if (aBegin.x < aEnd.x) {
		direction_x = 1;
	}
	else {
		direction_x = -1;
	}

	if (aBegin.y < aEnd.y) {
		direction_y = 1;
	}
	else {
		direction_y = -1;
	}

	//I used the bresenham's line drawing algorithm so we need to
	// initialize the slope error and current coordinates
	// slope_error is the difference between the actual position of the line and 
	//the ideal position of the line
	int slope_error = coord_length_x - coord_length_y;
	int x = static_cast<int>(aBegin.x);
	int y = static_cast<int>(aBegin.y);

	int width = aSurface.get_width();
	int height = aSurface.get_height();

	while (true) {
		// Check if the our coordinates x&y are within the bounds of the surface
		if (x >= 0 && x < width && y >= 0 && y < height) {
			// Set the pixel at the current coordinates to the specified color
			aSurface.set_pixel_srgb(x, y, aColor);
		}

		// If the current coordinates are equal to the end point (end of the vector), exit the loop
		//this line also handels the clipping aspect
		if (x == static_cast<int>(aEnd.x) && y == static_cast<int>(aEnd.y)) {
			break;
		}

		//we double the error 
		int slope_error2 = 2 * slope_error;

		// Calculate the step for the x and y directions
		//we give the line a direction "up"/"down" on the axis
		// the line moves a pixel
		//the line's position is closer to the ideal position 
		//in the y-direction (vertical) than in the x-direction (horizontal)
		if (slope_error2 > -coord_length_y) {
			slope_error = slope_error - coord_length_y;
			x = x + direction_x;
		}
		//the line's position is closer to the ideal position in the x-direction
		if (slope_error2 < coord_length_x) {
			slope_error = slope_error + coord_length_x;
			y = y + direction_y;
		}
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//task 1.4
	//we have to first draw the outline of the triangles
	draw_line_solid(aSurface, aP0, aP1, aColor);
	draw_line_solid(aSurface, aP1, aP2, aColor);
	draw_line_solid(aSurface, aP2, aP0, aColor);
}



void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor)
{
	//TODO: your implementation goes here
	//task 1.4
	//defined by 3 vertices & filled with solid colour
	//pick drawing method and it has to be O(N)
	//deal with clipping
	
	// method i used: scanline filling, fills diff triangles(flat bottom and top, or general ones)
	// Sort vertices by their y coordinate
	//aP0 left vertex, aP1 top vertex, aP2 right vertex 
	if (aP0.y > aP1.y) std::swap(aP0, aP1);
	if (aP0.y > aP2.y) std::swap(aP0, aP2);
	if (aP1.y > aP2.y) std::swap(aP1, aP2);

	// we check for degenerate triangles
	//no triangle is formed if the vertices lie on the horizontal line  
	if (aP0.y == aP2.y) return;

	int width = aSurface.get_width();
	int height = aSurface.get_height();

	// one handles top flat triangles
	if (aP0.y == aP1.y)
	{
		for (int y = static_cast<int>(aP0.y); y <= static_cast<int>(aP2.y); ++y)
		{
			//slope ecuation for starting x coordinates
			// Left Slope= (x1?x0)/(y1?y0)
			//Liner interpolation formula: startX=x0+(y?y0)*Left Slope
			int startLeftEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((aP2.x - aP0.x) / (aP2.y - aP0.y)));
			//slope ecuation for ending x coordinates
			// right Slope= (x2?x1)/(y2?y1)
			//Liner interpolation formula: endX=x1+(y?y1)*Right Slope
			int endRightEdgeX = static_cast<int>(aP1.x + (y - aP1.y) * ((aP2.x - aP1.x) / (aP2.y - aP1.y)));

			for (int x = startLeftEdgeX; x <= endRightEdgeX; ++x)
			{
				// Check if the our coordinates x&y are within the bounds of the surface
				if (x >= 0 && x < width && y >= 0 && y < height) {
					// Set the pixel at the current coordinates to the specified color
					aSurface.set_pixel_srgb(x, y, aColor);
				}
			}
		}
	}
	//one handles bottom flat triangles
	else if (aP1.y == aP2.y)
	{
		for (int y = static_cast<int>(aP0.y); y <= static_cast<int>(aP2.y); ++y)
		{
			int startLeftEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((aP1.x - aP0.x) / (aP1.y - aP0.y)));
			int endRightEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((aP2.x - aP0.x) / (aP2.y - aP0.y)));

			for (int x = startLeftEdgeX; x <= endRightEdgeX; ++x)
			{
				// Check if the our coordinates x&y are within the bounds of the surface
				if (x >= 0 && x < width && y >= 0 && y < height) {
					// Set the pixel at the current coordinates to the specified color
					aSurface.set_pixel_srgb(x, y, aColor);
				}
			}
		}
	}
	else
	{
		//this is for most(general) triangles that need to be split
		// we split the triangle into a bottom flat and top flat triangles
		Vec2f splitEdge;
		//Interpolated Value = Start Value +Total Distance/Fractional Distance*(End Value?Start Value)
		// we calculate x axis interpolated value
		float interpolatedValueX = aP0.x + ((float)(aP1.y - aP0.y) / (float)(aP2.y - aP0.y)) * (aP2.x - aP0.x);
		// we calculate y axis interpolated value
		float interpolatedValueY = aP1.y;

		// Split the triangle into a bottom flat and top flat triangles
		if (aP1.y == aP2.y)
		{
			// Handle bottom flat triangles
			splitEdge.x = interpolatedValueX;
			splitEdge.y = aP1.y;
		}
		else if (aP0.y == aP1.y)
		{
			// Handle top flat triangles
			splitEdge.x = interpolatedValueX;
			splitEdge.y = aP1.y;
		}
		else
		{
			// General case, other cases
			splitEdge.x = interpolatedValueX;
			splitEdge.y = interpolatedValueY;
		}

		// Handle bottom flat triangles
		for (int y = static_cast<int>(aP0.y); y <= static_cast<int>(aP1.y); ++y)
		{
			//again we do liner interpolation
			int startLeftEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((aP1.x - aP0.x) / (aP1.y - aP0.y)));
			int endRightEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((splitEdge.x - aP0.x) / (splitEdge.y - aP0.y)));

			for (int x = startLeftEdgeX; x <= endRightEdgeX; ++x)
			{
				// Check if the our coordinates x&y are within the bounds of the surface
				if (x >= 0 && x < width && y >= 0 && y < height) {
					// Set the pixel at the current coordinates to the specified color
					aSurface.set_pixel_srgb(x, y, aColor);
				}
			}
		}
		// Handle top flat triangles
		for (int y = static_cast<int>(aP1.y) + 1; y <= static_cast<int>(aP2.y); ++y)
		{
			int startLeftEdgeX = static_cast<int>(aP1.x + (y - aP1.y) * ((aP2.x - aP1.x) / (aP2.y - aP1.y)));
			int endRightEdgeX = static_cast<int>(aP0.x + (y - aP0.y) * ((splitEdge.x - aP0.x) / (splitEdge.y - aP0.y)));

			for (int x = startLeftEdgeX; x <= endRightEdgeX; ++x)
			{
				// Check if the our coordinates x&y are within the bounds of the surface
				if (x >= 0 && x < width && y >= 0 && y < height) {
					// Set the pixel at the current coordinates to the specified color
					aSurface.set_pixel_srgb(x, y, aColor);
				}
			}
		}
	}
}


//void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
//{
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//
//	//TODO: remove the following when you start your implementation
//	(void)aSurface; // Avoid warnings about unused arguments until the function
//	(void)aP0;   // is properly implemented.
//	(void)aP1;
//	(void)aP2;
//	(void)aColor;
//}


//void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
//{
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//	//TODO: your implementation goes here
//
//	//TODO: Remove the following when you start your implementation!
//	//TODO: This draws a solid triangle until you implement the function
//	//TODO: properly. (This is such that you can see the asteroids with solid
//	//TODO: shading until that point.)
//	draw_triangle_solid( aSurface, aP0, aP1, aP2, linear_to_srgb( aC0 ) );
//	(void)aC1;
//	(void)aC2;
//}

void draw_triangle_interp(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2)
{
	// Calculate the bounding box of the triangle
	//this helps with efficincy, limit the surface that is taking into consideration of randering
	// and clipping, make sure we don't generate pixels outside of the range of the surface
	Vec2f boundingBoxMin;
	boundingBoxMin.x = (aP0.x < aP1.x) ? ((aP0.x < aP2.x) ? aP0.x : aP2.x) : ((aP1.x < aP2.x) ? aP1.x : aP2.x);
	boundingBoxMin.y = (aP0.y < aP1.y) ? ((aP0.y < aP2.y) ? aP0.y : aP2.y) : ((aP1.y < aP2.y) ? aP1.y : aP2.y);

	// Calculate the bounding box of the triangle
	Vec2f boundingBoxMax;
	boundingBoxMax.x = (aP0.x > aP1.x) ? ((aP0.x > aP2.x) ? aP0.x : aP2.x) : ((aP1.x > aP2.x) ? aP1.x : aP2.x);
	boundingBoxMax.y = (aP0.y > aP1.y) ? ((aP0.y > aP2.y) ? aP0.y : aP2.y) : ((aP1.y > aP2.y) ? aP1.y : aP2.y);

	// Clamp: limit the value of the range to ensure the values are within valid bounds
	boundingBoxMin.x = (boundingBoxMin.x < 0) ? 0 : boundingBoxMin.x;
	boundingBoxMin.y = (boundingBoxMin.y < 0) ? 0 : boundingBoxMin.y;
	//these are used for cases such: when we have an image with valid pixel indices ranging from 0 to image_width - 1 
	// for the horizontal axis and 0 to image_height - 1 for the vertical axis
	//basically limmiting the bounds in which we calculate/draw pixels
	boundingBoxMax.x = (boundingBoxMax.x >= aSurface.get_width()) ? (aSurface.get_width() - 1) : boundingBoxMax.x;
	boundingBoxMax.y = (boundingBoxMax.y >= aSurface.get_height()) ? (aSurface.get_height() - 1) : boundingBoxMax.y;

	// calculating some constants
	float areaTriangle = (aP1.x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (aP1.y - aP0.y);
	//barycentric coordinate interpolation within a triangle, reciprocal of the area
	//a normalization factor that helps achieve correct barycentric interpolation within the triangle
	float inverseArea = 1.0f / areaTriangle;

	// Check if the area is 0, and skip the pixel/triangle
	if (areaTriangle == 0) {
		return;
	}

	// a nested loop that goes over the bounding box
	for (int x = static_cast<int>(boundingBoxMin.x); x <= static_cast<int>(boundingBoxMax.x); x++) {
		for (int y = static_cast<int>(boundingBoxMin.y); y <= static_cast<int>(boundingBoxMax.y); y++) {
			// here we finally calculate barycentric coordinates u, v
			//barycentric coordinates:b0,b1,b2
			float b0 = ((aP1.x - aP0.x) * (y - aP0.y) - (x - aP0.x) * (aP1.y - aP0.y)) * inverseArea;
			float b1 = ((x - aP0.x) * (aP2.y - aP0.y) - (aP2.x - aP0.x) * (y - aP0.y)) * inverseArea;
			float b2 = 1.0f - b0 - b1;

			if (b0 >= 0 && b1 >= 0 && b2 >= 0) {
				// this alg allows us to have different shades of colour, opposed to a single solid colour
				// so we need to interpolate colors using the barycentric coordinates
				ColorF interpolatedColour;
				interpolatedColour.r = (aC0.r * b0 + aC1.r * b1 + aC2.r * b2);
				interpolatedColour.g = (aC0.g * b0 + aC1.g * b1 + aC2.g * b2);
				interpolatedColour.b = (aC0.b * b0 + aC1.b * b1 + aC2.b * b2);

				// Set the pixel at the current coordinates to the specified color
				aSurface.set_pixel_srgb(x, y, linear_to_srgb(interpolatedColour));
			}
		}
	}
}

void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
