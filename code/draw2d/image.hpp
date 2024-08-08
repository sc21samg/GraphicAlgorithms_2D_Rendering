#ifndef IMAGE_HPP_ABCB2E1E_8092_422D_A0FE_80B26CC5E2D2
#define IMAGE_HPP_ABCB2E1E_8092_422D_A0FE_80B26CC5E2D2

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 1!
//
// For CW1, the image.hpp file must remain exactly as it is. In particular, you
// must not change the Image class interface or the declaration of blit_masked().

#include <memory>

#include <cassert>
#include <cstdlib>
#include <cstdint>

#include "forward.hpp"
#include "color.hpp"

#include "../vmlib/vec2.hpp"

/** ImageRGBA - an image that we can read from
 *
 * The ImageRGBA class encapsulates an image that we can read from. The class
 * itself abstract/pure virtual (see declaration of the destructor), meaning
 * that it cannot be instantiated directly.
 *
 * The image holds pixels of type ColorU8_sRGB_Alpha, i.e., RGBA data. The
 * alpha channel here is used, and encodes opacity (0 = transparent, 255 =
 * fully opaque).
 *
 * Use the `load_image()` method to load an image from disk. See image.cpp for
 * an implementation.
 *
 * REMINDER: DO NOT CHANGE THE INTERFACE OF THIS CLASS (see comment at the top).
 */
class ImageRGBA
{
	public:
		using Index = std::uint32_t; // See discussion in surface.hpp
	
	public:
		ImageRGBA();
		virtual ~ImageRGBA() = 0;

		// Not copyable (nor movable)
		ImageRGBA( ImageRGBA const& ) = delete;
		ImageRGBA& operator= (ImageRGBA const&) = delete;

	public:
		ColorU8_sRGB_Alpha get_pixel( Index aX, Index aY ) const;

		Index get_width() const noexcept;
		Index get_height() const noexcept;
	
		std::uint8_t* get_image_ptr() noexcept;
		std::uint8_t const* get_image_ptr() const noexcept;

		Index get_linear_index( Index aX, Index aY ) const noexcept;

	protected:
		Index mWidth, mHeight;
		std::uint8_t* mData;
};

/** Load image from disk
 *
 * Loads the image at aPath from disk and returns an ImageRGBA instance that 
 * holds the image data. 
 *
 * `load_image()` uses functions from stb_image.h internally, so it supports
 * the image formats that stb_image.h supports. See
 * https://github.com/nothings/stb/blob/master/stb_image.h
 * for details.
 */
std::unique_ptr<ImageRGBA> load_image( char const* aPath );

/** Blit image ImageRGBA into the provided Surface, at position aPosition
 *
 * REMINDER: DO NOT CHANGE THE PROTOTYPE OF THIS FUNCTION (see comment at the top)
 */
void blit_masked(
	Surface&,
	ImageRGBA const&,
	Vec2f aPosition
);

#include "image.inl"

#endif // IMAGE_HPP_ABCB2E1E_8092_422D_A0FE_80B26CC5E2D2

