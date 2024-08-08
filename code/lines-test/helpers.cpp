#include "helpers.hpp"

#include <algorithm>

#include <cassert>

#include "../draw2d/surface.hpp"

std::size_t max_row_pixel_count( Surface const& aSurface )
{
	std::size_t res = 0;

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
	{
		std::size_t inRow = 0;
		for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
		{
			// Surface::get_linear_index() can be implemented in slightly
			// different ways, with slightly different behaviour. Because
			// of this, this code can't rely on it. 
			//
			// In your own tests, where you know how you implemented
			// get_linear_index(), you can of course use it.
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( ptr[0] > 0 || ptr[1] > 0 || ptr[2] > 0 )
				++inRow;
		}

		res = std::max( inRow, res );
	}

	return res;
}

std::size_t max_col_pixel_count( Surface const& aSurface )
{
	std::size_t res = 0;

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		std::size_t inCol = 0;
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( ptr[0] > 0 || ptr[1] > 0 || ptr[2] > 0 )
				++inCol;
		}

		res = std::max( inCol, res );
	}

	return res;
}


std::array<std::size_t,9> count_pixel_neighbours( Surface const& aSurface )
{
	std::array<std::size_t,9> res;
	res.fill( 0 );

	static constexpr int kNeighbourOffsets[][2] = {
		{ -1, -1 }, {  0,  -1 }, { +1, -1 },
		{ -1,  0 },              { +1,  0 },
		{ -1, +1 }, {  0,  +1 }, { +1, +1 }
	};

	// There are exactly 8 neighbours in a 2D: left-right, top-bottom, and
	// diagonals (4).
	static_assert( sizeof(kNeighbourOffsets)/sizeof(kNeighbourOffsets[0]) == 8 );

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t yp = 0; yp < aSurface.get_height(); ++yp )
	{
		for( std::uint32_t xp = 0; xp < aSurface.get_width(); ++xp )
		{
			// Skip empty pixels
			auto const idx = yp*stride + (xp<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
				continue;

			// Count non-empty neighbours
			std::size_t neighbourCount = 0;
			for( auto const neighbour : kNeighbourOffsets )
			{
				auto const x = xp + neighbour[0];
				auto const y = yp + neighbour[1];

				// Outside? Note that this check is enough because uint32_t
				// is guaranteed to wrap on overflow.
				if( x >= aSurface.get_width() || y >= aSurface.get_height() )
					continue;

				auto const jdx = y*stride + (x<<2);
				auto const qtr = aSurface.get_surface_ptr() + jdx;

				if( qtr[0] > 0 || qtr[1] > 0 || qtr[2] > 0 )
					++neighbourCount;
			}

			assert( neighbourCount <= 8 );
			++res[neighbourCount];
		}
	}

	return res;
}
