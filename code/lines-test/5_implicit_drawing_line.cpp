#include <catch2/catch_amalgamated.hpp>
#include <algorithm>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Implicit Line Test", "[!mayfail]")
{
    Surface surface(200, 200);
    surface.clear();

    SECTION("Implicit Drawing Line")
    {
        // I have define an implicit line equation
        // For example y = 2x + 150 represents a line with a slope of 2 and y intercept at 150.

        // Calculate the endpoints based on the implicit line equation
        Vec2f x = { 50.f, 2 * 50.f + 150 };  
        Vec2f y = { 100.f, 2 * 100.f + 150 }; 

        // Draw the implicit line using the draw_line_solid function
        draw_line_solid(surface, x, y, { 255, 255, 255 });

        // we verify that the implicit line is drawn as expected
        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(pixels > 0);  // and at least one pixel should be drawn

        REQUIRE(1 == max_row_pixel_count(surface));
        REQUIRE(1 == max_col_pixel_count(surface));
    }
}
