#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp"


TEST_CASE( "sRGB conversion", "[interp][sRGB]" )
{
#	if DRAW2D_CFG_SRGB_MODE != DRAW2D_CFG_SRGB_EXACT
#		error "These tests require SRGB_MODE == SRGB_EXACT"
#	endif
	
	Surface surface( 16, 16 );
	surface.clear();

	SECTION( "linear midpoint" )
	{
		draw_triangle_interp( surface,
			{ 1.f, 15.f }, { 1.f, 1.f }, { 15.f, 1.f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f }
		);

		// I use the "find_most_red_pixel" to avoid making any assumptions on
		// how triangle drawing is implemented. In your own tests, you could
		// read back a single pre-determined pixel instead.
		auto const col = find_most_red_pixel( surface );
		REQUIRE( 188 == int(col.r) );
		REQUIRE( 188 == int(col.g) );
		REQUIRE( 188 == int(col.b) );
	}

	SECTION( "channels" )
	{
		draw_triangle_interp( surface,
			{ 1.f, 15.f }, { 1.f, 1.f }, { 15.f, 1.f },
			{ 0.212f, 0.051f, 0.527f },
			{ 0.212f, 0.051f, 0.527f },
			{ 0.212f, 0.051f, 0.527f }
		);

		auto const col = find_most_red_pixel( surface );
		REQUIRE( 127 == int(col.r) );
		REQUIRE( 64 == int(col.g) );
		REQUIRE( 192 == int(col.b) );
	}
}
