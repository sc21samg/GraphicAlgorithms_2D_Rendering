#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE("Two Adjacent Triangles", "[special][!mayfail]")
{
    Surface surface(640, 480);
    surface.fill({ 255, 255, 255 });

    // Draw the first triangle
    draw_triangle_interp(surface,
        { 50.f, 50.f },
        { 250.f, 250.f },
        { 450.f, 50.f },
        { 0.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f }
    );

    // Draw the second triangle sharing an edge with the first one
    draw_triangle_interp(surface,
        { 250.f, 250.f },
        { 450.f, 50.f },
        { 650.f, 250.f },
        { 0.f, 0.f, 1.f },
        { 1.f, 1.f, 0.f },
        { 0.f, 1.f, 1.f }
    );

    // Check the color of a pixel on the shared edge
    auto const col = find_most_red_pixel(surface);
    REQUIRE(255 == int(col.r));
    REQUIRE(255 == int(col.g));
    REQUIRE(255 == int(col.b));
}