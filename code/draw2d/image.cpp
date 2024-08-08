#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	//TODO: your implementation goes here
	//getting the source image width and height
	//this is usefull for iterating through each pixel and it's used in the nested for loops
	int blitImageWidth = aImage.get_width();
	int blitImageHeight = aImage.get_height();

	// we get through the pixels of the aImage
	//y coordinate dominant as the better option of reading pixels linearly
	for (int y = 0; y < blitImageHeight; ++y)
	{
		for (int x = 0; x < blitImageWidth; ++x)
		{
			//retriving the pixel colour from the image
			ColorU8_sRGB_Alpha pixelColour = aImage.get_pixel(x, y);

			// here we check if the alpha value of the pixel is greater or equal to 128
			//as the cw pdf suggests, this is alph amasking
			//the alpha value(transparancy) is stored in the "pixelColour.a"
			//we put the condition >=128 to make sure the ".a" value is opaque 
			//and it's used as a mask to determine if the pixel is good to be blitted(copied)
			//onto our surface or not
			if (pixelColour.a >= 128)
			{
				// we are calculating the destination position for the blit image
				//based on aPosition, where the image will be placed on the screen(surface) 
				//plus the size of each pixel added
				// I use static_cast<float>() to make sure I get the float precision and 
				//the arithmetic is consistant
				float blitDestX = aPosition.x + static_cast<float>(x);
				float blitDestY = aPosition.y + static_cast<float>(y);

				// we need to convert the ColorU8_sRGB_Alpha pixel to ColorU8_sRGB
				//to be accepted by set_pixel_srgb() function, aColour
				//again, this is similar to the original set_pixel_srgb() implementation
				ColorU8_sRGB convertedPixel;
				convertedPixel.r = pixelColour.r;
				convertedPixel.g = pixelColour.g;
				convertedPixel.b = pixelColour.b;

				// per usual we have to ensure that the destination position is 
				// within the bounds of aSurface
				if (blitDestX >= 0 && blitDestX < aSurface.get_width() && blitDestY >= 0 && blitDestY < aSurface.get_height())
				{
					// we blit(copy) the pixel to the destination position on aSurface
					// with the converted pixels
					aSurface.set_pixel_srgb(blitDestX, blitDestY, convertedPixel);
				}
			}
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
