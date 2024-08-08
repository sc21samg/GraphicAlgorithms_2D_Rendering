#include <catch2/catch_amalgamated.hpp>
#include <algorithm>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Negative Length Vertical Line", "[lines]")
{
    Surface surface(200, 200);
    surface.clear();

    SECTION("Line Negative Length")
    {
        draw_line_solid(surface,
            { 200.f, 200.f },  // we start at a higher point
            { 200.f, 150.f },   // and end at a lower point
            { 255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(0 == pixels);

        auto const counts = count_pixel_neighbours(surface);
        REQUIRE(0 == counts[0]); // No pixels should be drawn

     }
}
