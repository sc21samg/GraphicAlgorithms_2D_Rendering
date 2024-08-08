#include "background.hpp"

#include "../draw2d/image.hpp"

Background::Background( RNG& aRNG, std::uint32_t aImageWidth, std::uint32_t aImageHeight )
	: mFarField{
		{ aRNG, aImageWidth, aImageHeight, kFarColors[0], kFarDensities[0], kFarSpeedMults[0] },
		{ aRNG, aImageWidth, aImageHeight, kFarColors[1], kFarDensities[1], kFarSpeedMults[1] },
		{ aRNG, aImageWidth, aImageHeight, kFarColors[2], kFarDensities[2], kFarSpeedMults[2] }
	}
	, mNearField{ aRNG, aImageWidth, aImageHeight, kNearColor, kNearDensity, kNearSpeedMult }
{
	mEarthSprite = load_image( kEarthPath );
	mCurrentPosition = Vec2f{ 0.f, 0.f };
}

Background::~Background() = default;

void Background::update( Vec2f aPosition, Vec2f aMovementDelta )
{
	// Update particle fields
	for( auto& pf : mFarField )
		pf.update( aMovementDelta );

	mNearField.update( aMovementDelta );

	// Store current position
	mCurrentPosition = aPosition;
}

void Background::draw( Surface& aSurface )
{
	// Draw far field first
	for( auto const& pf : mFarField )
		pf.draw( aSurface );

	// Draw earth sprite
	blit_masked( aSurface, *mEarthSprite, kEarthCoord - mCurrentPosition );

	// Draw near field = dirt layer
	mNearField.draw( aSurface );
}

void Background::resize( std::uint32_t aImageWidth, std::uint32_t aImageHeight )
{
	for( auto& pf : mFarField )
		pf.resize( aImageWidth, aImageHeight );

	mNearField.resize( aImageWidth, aImageHeight );
}
