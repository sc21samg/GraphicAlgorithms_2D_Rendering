#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "1px wide lines", "[thin]" )
{
	// Note: this code is run before each of the SECTION()s below run! This
	// gives each of the SECTION()s a fresh+cleared Surface to work with.
	Surface surface( 128, 128 );
	surface.clear();

	SECTION( "horizontal" )
	{
		draw_line_solid( surface,
			{ 10.f, 47.f },
			{ 110.f, 47.f },
			{ 255, 255, 255 }
		);

		REQUIRE( max_row_pixel_count( surface ) > 1 );
		REQUIRE( 1 == max_col_pixel_count( surface ) );
	}
	SECTION( "vertical" )
	{
		draw_line_solid( surface,
			{ 64.f, 28.f },
			{ 64.f, 100.f },
			{ 255, 255, 255 }
		);

		REQUIRE( 1 == max_row_pixel_count( surface ) );
		REQUIRE( max_col_pixel_count( surface ) > 1 );
	}

	SECTION( "x-major" )
	{
		draw_line_solid( surface,
			{ 1.f, 20.f },
			{ 127.f, 60.f },
			{ 255, 255, 255 }
		);

		REQUIRE( max_row_pixel_count( surface ) > 1 );
		REQUIRE( 1 == max_col_pixel_count( surface ) );
	}
	SECTION( "y-major" )
	{
		draw_line_solid( surface,
			{ 79.f, 28.f },
			{ 70.f, 100.f },
			{ 255, 255, 255 }
		);

		REQUIRE( 1 == max_row_pixel_count( surface ) );
		REQUIRE( max_col_pixel_count( surface ) > 1 );
	}

	SECTION( "diagonal" )
	{
		draw_line_solid( surface,
			{ 10.f, 10.f },
			{ 100.f, 100.f },
			{ 255, 255, 255 }
		);

		REQUIRE( 1 == max_row_pixel_count( surface ) );
		REQUIRE( 1 == max_col_pixel_count( surface ) );
	}

}
