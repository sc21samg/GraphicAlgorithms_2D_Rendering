#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Multicolour Scalene Triangle", "[special][!mayfail]")
{
    Surface surface(320, 240);
    surface.fill({ 255, 255, 255 });

    draw_triangle_interp(surface,
        { 50.f, 50.f },
        { 250.f, 250.f },
        { 450.f, 100.f },
        { 0.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f }
    );

    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}