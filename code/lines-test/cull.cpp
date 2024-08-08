#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Fully offscreen", "[cull]" )
{
	Surface surface( 100, 100 );
	surface.clear();

	// This has a few simple cases.
	
	SECTION( "left" )
	{
		draw_line_solid( surface,
			{ -10.f, 5.f },
			{ -20.f, 5.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 0 == pixels );
	}

	SECTION( "right" )
	{
		draw_line_solid( surface,
			{ 120.f, 95.f },
			{ 110.f, 95.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 0 == pixels );
	}

	SECTION( "above" )
	{
		draw_line_solid( surface,
			{ 50.f, 102.f },
			{ 60.f, 900.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 0 == pixels );
	}

	SECTION( "below" )
	{
		draw_line_solid( surface,
			{ 17.f, -100.f },
			{ 92.f, -7.f },
			{ 255, 255, 255 }
		);

		auto const pixels = max_row_pixel_count( surface );
		REQUIRE( 0 == pixels );
	}

	// Not tested: diagonals, etc.
}
