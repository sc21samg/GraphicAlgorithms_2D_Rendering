#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Zero length", "[special]" )
{
	Surface surface( 50, 50 );
	surface.clear();

	// This is a zero-lenght line.
	//
	// The exercise does not specify what should happen in this case. The two
	// sensible options are: 
	//   - draw nothing
	//   - draw a single pixel
	draw_line_solid( surface,
		{ 25.f, 25.f },
		{ 25.f, 25.f },
		{ 255, 255, 255 }
	);

	auto const counts = count_pixel_neighbours( surface );

	// Either: no pixel with zero neighbours (nothing drawn) or one pixel with
	// zero neighbours (single pixel drawn)
	REQUIRE( (0 == counts[0] || 1 == counts[0]) );

	// No other pixels should exist
	for( std::size_t i = 1; i < counts.size(); ++i )
		REQUIRE( 0 == counts[i]  );
}
