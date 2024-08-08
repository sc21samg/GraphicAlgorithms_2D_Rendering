#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Fullscreen", "[special]" )
{
	
	Surface surface( 320, 240 );
	surface.fill( { 255, 255, 255 } );

	draw_triangle_solid( surface,
		{ -5.f, -5.f }, { 700.f, -5.f }, { -5.f, 500.f },
		{ 0, 0, 0 }
	);

	auto const col = find_most_red_pixel( surface );
	REQUIRE( 0 == int(col.r) );
	REQUIRE( 0 == int(col.g) );
	REQUIRE( 0 == int(col.b) );
}
