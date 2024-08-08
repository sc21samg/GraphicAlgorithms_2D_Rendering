#include <catch2/catch_amalgamated.hpp>
#include <algorithm>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Intersecting Lines", "[lines]")
{
    Surface surface(640, 480);
    surface.clear();

    SECTION("Intersection Test")
    {
        draw_line_solid(surface,
            { 150.f, 150.f },
            { 400.f, 400.f },
            { 255, 255, 255 }
        );

        draw_line_solid(surface,
            { 400.f, 150.f },
            { 150.f, 400.f },
            { 255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count(surface);
        //each pixel should count one pixel each
        REQUIRE(2 == pixels);
    }
}