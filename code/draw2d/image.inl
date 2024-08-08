inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

	//TODO: your implementation goes here
	Index linearIndex = get_linear_index(aX, aY);

	// get the pixel data from the image
	const ColorU8_sRGB_Alpha* imagePixelData = reinterpret_cast<const ColorU8_sRGB_Alpha*>(mData);
	//return the pixel colour located at the x, y coordinates
	return imagePixelData[linearIndex];
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	//TODO: your implementation goes here
	// I calculate the linear index for the pixel in the image data array
	//same way as set_pixel_srgb() function without x4
	Index linearIndex = aY * mWidth + aX;

	return linearIndex;
}
