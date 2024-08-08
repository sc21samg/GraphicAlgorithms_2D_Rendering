#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/surface.hpp"

namespace
{

	// DDA (Digital Differential Analyzer) line drawing algorithm with floats
	void draw_line_dda_floats_(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
	{
		//initialisation of endpoints of the line
		float x1 = aBegin.x;
		float y1 = aBegin.y;
		float x2 = aEnd.x;
		float y2 = aEnd.y;
		//differences between x,y coordinates
		float dx = x2 - x1;
		float dy = y2 - y1;

		// we determine the number of steps needed based on the maximum difference in coordinates
		//maximum between the 2 absolutes
		float steps = std::max(std::abs(dx), std::abs(dy));

		// we calculate increments for x and y
		//based on no of steps
		float incrementX = dx / steps;
		float incrementY = dy / steps;

		float x = x1;
		float y = y1;

		// we have to round coordinates to the nearest pixel
		//making sure that the drawing of specific pixels is being done accordingly
		int ix = static_cast<int>(std::round(x));
		int iy = static_cast<int>(std::round(y));

		int width = aSurface.get_width();
		int height = aSurface.get_height();

		// Draw the line by setting pixels along the line
		for (int i = 0; i <= steps; ++i) {
			// Check if the pixel is within the bounds of the surface
			if (ix >= 0 && ix < width && iy >= 0 && iy < height) {
				// Set the pixel at the current coordinates to the specified color
				aSurface.set_pixel_srgb(ix, iy, aColor);
			}

			// with each step we move to the next pixel along the line
			x = x + incrementX;
			y = y + incrementY;

			// again we round the coordinates to the nearest pixel
			ix = static_cast<int>(std::round(x));
			iy = static_cast<int>(std::round(y));
		}
	}

	// Improved DDA (integer-only) line drawing algorithm
	void draw_line_dda_integer_(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
	{
		//making sure that the values extracted are integers
		int x1 = static_cast<int>(aBegin.x);
		int y1 = static_cast<int>(aBegin.y);
		int x2 = static_cast<int>(aEnd.x);
		int y2 = static_cast<int>(aEnd.y);
		//differences between x,y coordinates
		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);
		// determining the direction of the line
		int directionX = (x1 < x2) ? 1 : -1;
		int directionY = (y1 < y2) ? 1 : -1;
		//error initialisation, diff between x,y coordinates
		int error = dx - dy;

		int width = aSurface.get_width();
		int height = aSurface.get_height();

		while (true)
		{
			// Set the pixel at the current coordinates to the specified color
			if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
			{
				aSurface.set_pixel_srgb(x1, y1, aColor);
			}

			// If the current coordinates are equal to the end point, exit the loop
			if (x1 == x2 && y1 == y2)
			{
				break;
			}
			//we double the error so moves along the line in a way 
			//that minimizes visual artifacts and maintains a relatively straight trajectory
			int error2 = 2 * error;

			// If the next step in x direction is smaller than in y direction
			//it increments x1 and updates error
			if (error2 > -dy)
			{
				error = error - dy;
				x1 = x1 + directionX;
			}

			// If the next step in y direction is smaller than in x direction
			//it increments y1 and updates error
			if (error2 < dx)
			{
				error = error + dx;
				y1 = y1 + directionY;
			}
		}
	}

	void a_my_original_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//simple, straight line
			draw_line_solid(surface,
				{ 10.f, 170.f },
				{ 110.f, 170.f },
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}

	// Benchmark function for the DDA algorithm
	void a_my_dda_floats_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//simple, straight line
			draw_line_dda_floats_(surface,
				{ 10.f, 170.f },
				{ 110.f, 170.f },
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}
	void a_my_dda_integer_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//simple, straight line
			draw_line_dda_integer_(surface,
				{ 10.f, 170.f },
				{ 110.f, 170.f },
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}
	//////////////////////

	void b_my_original_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//a clipping line
			// Extends out of screen
			draw_line_solid(surface,
				{ width / 2.f, height / 2.f }, { width + 10.f, height / 2.f },
				{ 255, 255, 0 }
			);
			benchmark::ClobberMemory();
		}
	}

	// Benchmark function for the DDA algorithm
	void b_my_dda_floats_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//a clipping line
			// Extends out of screen
			draw_line_dda_floats_(surface,
				{ width / 2.f, height / 2.f }, { width + 10.f, height / 2.f },
				{ 255, 255, 0 }
			);
			benchmark::ClobberMemory();
		}
	}
	void b_my_dda_integer_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//a clipping line
			// Extends out of screen
			draw_line_dda_integer_(surface,
				{ width / 2.f, height / 2.f }, { width + 10.f, height / 2.f },
				{ 255, 255, 0 }
			);
			benchmark::ClobberMemory();
		}
	}
	//////////////////////////////////////

	void c_my_original_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//steep diagonal line
			draw_line_solid(surface,
				{ 10.f, 10.f },  
				{ 110.f, 100.f },  
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}

	// Benchmark function for the DDA algorithm
	void c_my_dda_floats_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//steep diagonal line
			draw_line_dda_floats_(surface,
				{ 10.f, 10.f },
				{ 110.f, 100.f },
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}
	void c_my_dda_integer_line_alg_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{//steep diagonal line
			draw_line_dda_integer_(surface,
				{ 10.f, 10.f },
				{ 110.f, 100.f },
				{ 255, 255, 255 }
			);
			benchmark::ClobberMemory();
		}
	}
}

BENCHMARK(a_my_original_line_alg_)
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;

BENCHMARK(a_my_dda_floats_line_alg_)
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;

BENCHMARK(a_my_dda_integer_line_alg_)
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;
//////
BENCHMARK(b_my_original_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(b_my_dda_floats_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(b_my_dda_integer_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;
///
BENCHMARK(c_my_original_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(c_my_dda_floats_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(c_my_dda_integer_line_alg_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;


BENCHMARK_MAIN();
