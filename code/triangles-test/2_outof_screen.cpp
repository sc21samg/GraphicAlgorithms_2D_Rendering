#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Triangle Fully Outside Screen", "[special][!mayfail]")
{
    Surface surface(320, 240);
    surface.fill({ 255, 255, 255 });

    draw_triangle_interp(surface,
        { -100.f, 300.f },
        { -50.f, 350.f },
        { -200.f, 400.f },
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f }
    );

    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}
