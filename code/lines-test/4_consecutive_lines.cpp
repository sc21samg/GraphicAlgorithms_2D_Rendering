#include <catch2/catch_amalgamated.hpp>
#include <algorithm>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Consecutive Lines with No Gaps", "[lines]")
{
    Surface surface(200, 200);
    surface.clear();

    SECTION("Consecutive Lines Test")
    {
		draw_line_solid(surface,
			{ 50.f, 100.f },
			{ 100.f, 100.f },
			{ 255, 255, 255 }
		);

		// we take the end point of the first line and use it as the start point for the second line
		Vec2f startPoint = { 100.f, 100.f };

		draw_line_solid(surface,
			startPoint,  // we start at the end of the first line
			{ 150.f, 100.f },  // and extend to a new end point
			{ 255, 255, 255 }
		);

        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(1 == pixels);  // 1 is the expected number of pixels for the two consecutive lines
    }
}
