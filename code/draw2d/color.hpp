#ifndef COLOR_HPP_1239E14D_0FDD_4FA5_BF6B_ADB891884682
#define COLOR_HPP_1239E14D_0FDD_4FA5_BF6B_ADB891884682

#include <cmath>
#include <cstdint>

/* Compile-time configuration:
 * Pick between different approximations for color conversion from linear RGB
 * to sRGB. EXACT uses the correct published sRGB coversion method [1-3]. FAST
 * FASTER approximate the coversion with a simpler (and probably faster) method.
 * FAST uses just a gamma curve with an exponent of 2.4. FASTER approximates 
 * this further by using an exponent of 2.0 (=square and square root).
 *
 * [1] https://www.khronos.org/registry/DataFormat/specs/1.3/dataformat.1.3.html#TRANSFER_SRGB
 * [2] https://en.wikipedia.org/wiki/SRGB
 * [2] https://gamedev.stackexchange.com/q/92015
 */
#define DRAW2D_CFG_SRGB_EXACT 1
#define DRAW2D_CFG_SRGB_FAST 2
#define DRAW2D_CFG_SRGB_FASTER 3

// The default is to use EXACT. You can change the following to pick a 
// different method.
#define DRAW2D_CFG_SRGB_MODE DRAW2D_CFG_SRGB_EXACT


/** Linear RGB color
 *
 * Represents a linear RGB color. Each component (r, g, b) must be in the range
 * [0, 1]. Each component is stored as a 32-bit float, for a total of 12 bytes.
 */
struct ColorF
{
	float r;
	float g;
	float b;
};

/** sRGB color
 *
 * Stores a sRGB color as three 8-bit values, for a total of 24-bits or 3
 * bytes.  Each component utilizes the full range of the `std::uint8_t` type,
 * i.e., [0, 255].
 */
struct ColorU8_sRGB
{
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
};

/** sRGB color with alpha channel
 *
 * Stores a sRGB color with an alpha channel as four 8-bit values, for a total
 * of 32-bits or 4 bytes. Each component utilizes the full range of the
 * `std::uint8_t` type, i.e., [0, 255]. Note that the alpha channel is linear.
 * It is not transformed according to the sRGB mapping.
 */
struct ColorU8_sRGB_Alpha
{
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
	std::uint8_t a;
};

// Helper functions to convert between linear RGB and sRGB values:

std::uint8_t linear_to_srgb( float aValue ) noexcept;
float linear_from_srgb( std::uint8_t aValue ) noexcept;

ColorU8_sRGB linear_to_srgb( ColorF const& ) noexcept;
ColorF linear_from_srgb( ColorU8_sRGB const& ) noexcept;

#include "color.inl"
#endif // COLOR_HPP_1239E14D_0FDD_4FA5_BF6B_ADB891884682
