#ifndef SURFACE_HPP_C464AD04_D6E0_459B_BDF9_51C65C009BF3
#define SURFACE_HPP_C464AD04_D6E0_459B_BDF9_51C65C009BF3

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 1!
//
// For CW1, the surface.hpp file must remain exactly as it is. In particular,
// you must not change the Surface class interface.

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include "color.hpp"

/** Surface - an image that we can draw to
 *
 * The surface class enacpsulates an image that we can draw to. It provides
 * minimal functionality to do so.
 *
 * The image data stored by the surface is stored as an 32-bit RGBx format.
 * Each channel (r, g, b) uses 8 bits. There is an additional 8 bits of padding
 * for each pixel, making each pixel exactly 32-bits in size.
 *
 * The image data is further stored in the sRGB space.
 *
 * REMINDER: DO NOT CHANGE THE INTERFACE OF THIS CLASS (see comment at the top).
 */
class Surface final
{
	public:
		//using Index = std::size_t;
		using Index = std::uint32_t; // See discussion below.
	
	public:
		Surface( Index aWidth, Index aHeight );
		~Surface();

		// The surface is "move-only". It cannot be copied, but ownership of
		// the surface's data can be transferred from one Surface instance to
		// another. Study C++'s move semantics for further details.
		Surface( Surface const& ) = delete;
		Surface& operator= (Surface const&) = delete;

		Surface( Surface&& ) noexcept;
		Surface& operator= (Surface&&) noexcept;

	public:
		// Clear surface image data to (0,0,0) = black
		void clear() noexcept;

		// Clear surface to specified color
		void fill( ColorU8_sRGB ) noexcept;
	
		// Set the pixel at index (aX,aY) to the specified color
		void set_pixel_srgb( Index aX, Index aY, ColorU8_sRGB const& );

		// Get pointer to surface image data. This is mainly used when drawing
		// the surface's contents to the screen. You must not use these functions
		// when implementing your drawing functions.
		std::uint8_t const* get_surface_ptr() const noexcept;

		// Return surfac width
		Index get_width() const noexcept;

		// Return surface height
		Index get_height() const noexcept;

		// Compute the linear index of pixel (aX,aY)
		Index get_linear_index( Index aX, Index aY ) const noexcept;

	private:
		std::uint8_t* mSurface; // Surface image data, sRGB, stored as RGBx8
		Index mWidth, mHeight; // Surface width and height in pixels

	/* Extra discussion re: Index type.
	 *
	 * The default choice for Index is (for now) std::uint32_t. I originally
	 * wrote the class with the std::size_t type, which is the type I recommend
	 * for use as indices by default. (Pixel coordinates are ultimately just
	 * indices into a memory area.)
	 *
	 * I would normally use recommend std::size_t for indexing because it has
	 * the same size as pointers. This reduces extra conversations (e.g.,
	 * extending to 64 bit on 64 bit platforms). This isn't entirely
	 * uncontroversial, but I'll stay away from long discussions on signed vs.
	 * unsigned for now. *My* observation is that unsigned pointer-sized
	 * integers tend to result in very good code.
	 *
	 * However, in this specific case, it turns out that std::size_t is
	 * *likely* to introduce extra overheads in x86(_64), which is probably the
	 * most common platform out there. It is very likely that at one point or
	 * another, somebody will try to convert a float to the Index type.
	 *
	 * Converting float to 64-bit integers is quite a bit more expensive than
	 * to 32 bit integers. See Compiler Explorer:
	 *
	 * https://gcc.godbolt.org/z/sMbPzcEPd
	 *
	 * Converting a float to a 32-bit unsigned is a single [v]cvttss2si 
	 * instruction (SSE/AVX). Read vcttss2si as:
	 *   - v: VEX encoding, used with AVX. vcvt... = AVX, cvt... = SSE
	 *   - cvt: convert
	 *   - t: with truncation
	 *   - ss: *s*calar *s*ingle precision floating point number
	 *   - 2: "to"
	 *   - si: *s*calar *i*nteger
	 *  (The AT&T syntax used by GCC/Clang adds a 'q' suffix which indicates
	 *  that the destination register is a "quadword" = 64 bit.)
	 *
	 * Converting the float to a 64-bit integer takes a bunch of instructions,
	 * in some cases including a branch.
	 *
	 * This does not appear to be an issue with ARM64. You may want to
	 * experiment with Index = std::size_t there.
	 */
};

#include "surface.inl"
#endif // SURFACE_HPP_C464AD04_D6E0_459B_BDF9_51C65C009BF3
