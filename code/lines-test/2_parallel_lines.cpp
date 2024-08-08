#include <catch2/catch_amalgamated.hpp>
#include <algorithm>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Parallel Lines", "[lines]")
{
    Surface surface(640, 480);
    surface.clear();

    SECTION("Parallel Lines Test, far apart")
    {
        draw_line_solid(surface,
            { 40.f, 150.f },
            { 700.f, 150.f },
            { 255, 255, 255 }
        );

        draw_line_solid(surface,
            { 40.f, 200.f },
            { 700.f, 200.f },
            { 255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(2 == pixels);
    }

    SECTION("Parallel Lines Test, 0 pixels apart")
    {
        //this is basically overlapping the lines
        draw_line_solid(surface,
            { 140.f, 250.f },
            { 900.f, 250.f },
            { 255, 255, 255 }
        );

        draw_line_solid(surface,
            { 140.f, 250.f },
            { 900.f, 250.f },
            { 255, 255, 255 }
        );              

        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(1 == pixels); // with the overlapping there should be only one pixel counted
    }

    SECTION("Parallel Lines Test, 1 pixel apart")
    {
        //you can see the line is doubled
        draw_line_solid(surface,
            { 240.f, 350.f },
            { 1000.f, 350.f },
            { 255, 255, 255 }
        );

        draw_line_solid(surface,
            { 240.f, 351.f },
            { 1000.f, 351.f },
            { 255, 255, 255 }
        );

        auto const pixels = max_col_pixel_count(surface);
        REQUIRE(2 == pixels);
    }
}
