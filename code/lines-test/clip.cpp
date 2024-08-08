#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Partially offscreen", "[clip]" )
{
	Surface surface( 640, 480 );
	surface.clear();

	// Only one sample. You may want to extend this with your own variants.
	// Minimally, you should test with lines that aren't axis aligned.
	
	SECTION( "go right" )
	{
		draw_line_solid( surface,
			{ 1.f, 240.f },
			{ 1920.f, 240.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_col_pixel_count( surface );
		REQUIRE( 1 == pixels );
	}
}
