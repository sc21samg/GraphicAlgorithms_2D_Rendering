inline
std::uint8_t linear_to_srgb( float aValue ) noexcept
{
#	if DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_EXACT
	if( aValue < 0.0031308f )
		return std::uint8_t(255.f * 12.92f * aValue + 0.5f);
	
	return std::uint8_t(255.f * (1.055f * std::pow( aValue, 1.f/2.4f ) - 0.055f) + 0.5f);

#	elif DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_FAST
	return std::uint8_t(255.f * std::pow( aValue, 1.f/2.4f ) + 0.5f);

#	elif DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_FASTER
	return std::uint8_t(255.f * std::sqrt( aValue ) + 0.5f);

#	endif // ~ DRAW2D_CFG_SRGB_MODE
}

inline
float linear_from_srgb( std::uint8_t aValue ) noexcept
{
	float const fvalue = float(aValue) / 255.f;

#	if DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_EXACT
	if( fvalue < 0.04045f )
		return (1.f/12.92f) * fvalue;

	return std::pow( (1.f/1.055f) * (fvalue + 0.055f), 2.4f );

#	elif DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_FAST
	return std::pow( fvalue, 2.4f );

#	elif DRAW2D_CFG_SRGB_MODE == DRAW2D_CFG_SRGB_FASTER
	return fvalue * fvalue;

#	endif // ~ DRAW2D_CFG_SRGB_MODE

}

inline
ColorU8_sRGB linear_to_srgb( ColorF const& aColor ) noexcept
{
	return ColorU8_sRGB{
		linear_to_srgb( aColor.r ),
		linear_to_srgb( aColor.g ),
		linear_to_srgb( aColor.b )
	};
}

inline
ColorF linear_from_srgb( ColorU8_sRGB const& aColor ) noexcept
{
	return ColorF{
		linear_from_srgb( aColor.r ),
		linear_from_srgb( aColor.g ),
		linear_from_srgb( aColor.b )
	};
}
