#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstring> // for std::memcpy

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

namespace
{
	void a_default_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface( width, height );
		surface.clear();

		auto source = load_image( "assets/earth.png" );
		assert( source );

		for( auto _ : aState )
		{
			blit_masked( surface, *source, {0.f, 0.f} );

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}


	void a_my_original_blit_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/earth.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					// Retrieve the pixel color from the image
					ColorU8_sRGB_Alpha pixelColour = source->get_pixel(x, y);

					// Convert the ColorU8_sRGB_Alpha pixel to ColorU8_sRGB
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = pixelColour.r;
					convertedPixel.g = pixelColour.g;
					convertedPixel.b = pixelColour.b;

					// Calculate the destination position for the blit image
					float blitDestX = static_cast<float>(x);
					float blitDestY = static_cast<float>(y);

					// Ensure that the destination position is within the bounds of aSurface
					if (blitDestX >= 0 && blitDestX < width && blitDestY >= 0 && blitDestY < height)
					{
						// Blit (copy) the pixel to the destination position on aSurface
						surface.set_pixel_srgb(blitDestX, blitDestY, convertedPixel);
					}
				}
			}

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically possible.)
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 3 * aState.iterations());
	}



	void a_my_no_alpha_masking_blit_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/earth.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					// Copy pixel from source to target without alpha masking
					auto sourcePixel = source->get_pixel(x, y);
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = sourcePixel.r;
					convertedPixel.g = sourcePixel.g;
					convertedPixel.b = sourcePixel.b;
					surface.set_pixel_srgb(x, y, convertedPixel);
				}
			}

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically possible.)
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}


//#include <cstring> // for std::memcpy

	void a_my_memcpy_calls_blit_(benchmark::State& aState)
	{

		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/earth.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				// Calculate the source and destination pointers for each line
				const auto sourceLine = source->get_pixel(0, y);
				auto* destLine = reinterpret_cast<ColorU8_sRGB*>(const_cast<std::uint8_t*>(surface.get_surface_ptr()));

				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = sourceLine.r;
					convertedPixel.g = sourceLine.g;
					convertedPixel.b = sourceLine.b;

					// Copy the converted pixel to the destination line
					std::memcpy(destLine + x, & convertedPixel, sizeof(ColorU8_sRGB));
					//std::memcpy(&destLine[x], &convertedPixel, sizeof(ColorU8_sRGB));
				}
			}

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically possible.)
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}
}

BENCHMARK(a_default_blit_earth_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(a_my_original_blit_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(a_my_no_alpha_masking_blit_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;


BENCHMARK(a_my_memcpy_calls_blit_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

namespace
{
	void b_default_blit_earth_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/cute_snail.png");
		assert(source);

		for (auto _ : aState)
		{
			blit_masked(surface, *source, { 0.f, 0.f });

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}


	void b_my_original_blit_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/cute_snail.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					// Retrieve the pixel color from the image
					ColorU8_sRGB_Alpha pixelColour = source->get_pixel(x, y);

					// Convert the ColorU8_sRGB_Alpha pixel to ColorU8_sRGB
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = pixelColour.r;
					convertedPixel.g = pixelColour.g;
					convertedPixel.b = pixelColour.b;

					// Calculate the destination position for the blit image
					float blitDestX = static_cast<float>(x);
					float blitDestY = static_cast<float>(y);

					// Ensure that the destination position is within the bounds of aSurface
					if (blitDestX >= 0 && blitDestX < width && blitDestY >= 0 && blitDestY < height)
					{
						// Blit (copy) the pixel to the destination position on aSurface
						surface.set_pixel_srgb(blitDestX, blitDestY, convertedPixel);
					}
				}
			}
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 3 * aState.iterations());
	}


	void b_my_no_alpha_masking_blit_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/cute_snail.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					// Copy pixel from source to target without alpha masking
					auto sourcePixel = source->get_pixel(x, y);
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = sourcePixel.r;
					convertedPixel.g = sourcePixel.g;
					convertedPixel.b = sourcePixel.b;
					surface.set_pixel_srgb(x, y, convertedPixel);
				}
			}
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}

#include <cstring> // for std::memcpy

	void b_my_memcpy_calls_blit_(benchmark::State& aState)
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Surface surface(width, height);
		surface.clear();

		auto source = load_image("assets/cute_snail.png");
		assert(source);

		auto const maxBlitX = std::min(width, source->get_width());
		auto const maxBlitY = std::min(height, source->get_height());

		for (auto _ : aState)
		{
			for (std::size_t y = 0; y < maxBlitY; ++y)
			{
				// Calculate the source and destination pointers for each line
				const auto sourceLine = source->get_pixel(0, y);
				auto* destLine = reinterpret_cast<ColorU8_sRGB*>(const_cast<std::uint8_t*>(surface.get_surface_ptr()));

				for (std::size_t x = 0; x < maxBlitX; ++x)
				{
					ColorU8_sRGB convertedPixel;
					convertedPixel.r = sourceLine.r;
					convertedPixel.g = sourceLine.g;
					convertedPixel.b = sourceLine.b;

					// Copy the converted pixel to the destination line
					std::memcpy(destLine + x, &convertedPixel, sizeof(ColorU8_sRGB));
				}
			}
			benchmark::ClobberMemory();
		}

		// SetBytesProcessed for memory bandwidth information
		aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	}
}

BENCHMARK(b_default_blit_earth_ )
	->Args( { 320, 240 } )
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args( { 7680, 4320 } )
;

BENCHMARK(b_my_original_blit_)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(b_my_no_alpha_masking_blit_)
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;


BENCHMARK(b_my_memcpy_calls_blit_)
	->Args({ 320, 240 })
	->Args({ 1280, 720 })
	->Args({ 1920, 1080 })
	->Args({ 7680, 4320 })
;

BENCHMARK_MAIN();
