#include "helpers.hpp"

#include "../draw2d/color.hpp"
#include "../draw2d/surface.hpp"


ColorU8_sRGB find_most_red_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 0, 0, 0 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			// Not really needed.
			//if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
			//	continue;

			if( ptr[0] >= ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;
}
ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 255, 255, 255 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
				continue;

			if( ptr[0] < ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;

}

