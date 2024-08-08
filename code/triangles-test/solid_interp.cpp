#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Solid vs interp", "[special]" )
{
	Surface surface( 320, 240 );
	surface.clear();

	// We'd expected that draw_triangle_solid() draws the same triangle as
	// draw_triangle_interp()!

	SECTION( "interp then solid" )
	{
		draw_triangle_interp( surface,
			{ 10.f, 5.f }, { 300.f, 50.f }, { 17.f, 210.f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f },
			{ 0.5f, 0.5f, 0.5f }
		);

		// Ensure something's been drawn.
		auto const col = find_most_red_pixel( surface );
		REQUIRE( 0 != int(col.r) );
		REQUIRE( 0 != int(col.g) );
		REQUIRE( 0 != int(col.b) );

		// Draw over same triangle
		draw_triangle_solid( surface,
			{ 10.f, 5.f }, { 300.f, 50.f }, { 17.f, 210.f },
			{ 0, 0, 0 }
		);

		// Should be cleared again.
		auto const col2 = find_most_red_pixel( surface );
		REQUIRE( 0 == int(col2.r) );
		REQUIRE( 0 == int(col2.g) );
		REQUIRE( 0 == int(col2.b) );
	}

	SECTION( "solid then interp" )
	{
		draw_triangle_solid( surface,
			{ 10.f, 5.f }, { 300.f, 50.f }, { 17.f, 210.f },
			{ 3, 13, 37 }
		);

		auto const col = find_most_red_pixel( surface );
		REQUIRE( 0 != int(col.r) );
		REQUIRE( 0 != int(col.g) );
		REQUIRE( 0 != int(col.b) );

		draw_triangle_interp( surface,
			{ 10.f, 5.f }, { 300.f, 50.f }, { 17.f, 210.f },
			{ 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f }
		);

		auto const col2 = find_most_red_pixel( surface );
		REQUIRE( 0 == int(col2.r) );
		REQUIRE( 0 == int(col2.g) );
		REQUIRE( 0 == int(col2.b) );
	}

}
